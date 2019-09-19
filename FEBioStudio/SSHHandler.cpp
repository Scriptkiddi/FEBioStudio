#ifdef HAS_SSH
#include <string.h>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <QMessageBox>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QInputDialog>

#include "SSHHandler.h"
#include <FSCore/FSDir.h>
#include "Logger.h"
#include "Encrypter.h"

#include <iostream>

#define MAX_XFER_BUF_SIZE 16384

#ifdef WIN32
#define S_IRWXU (0400|0200|0100)
#endif


CSSHHandler::CSSHHandler (CFEBioJob* job) : job(job), passwdLength(-1) // @suppress("Class members should be properly initialized")
{
	// Get local .feb file name
	std::string localFile = FSDir::toAbsolutePath(job->GetFileName());

	QFileInfo info(localFile.c_str());
	std::string baseName = info.baseName().toStdString();

	// Get remote base file name
	remoteFileBase = job->GetLaunchConfig()->remoteDir + "/" + baseName;
}

void CSSHHandler::Update(CLaunchConfig& oldConfig)
{
	if(!job->GetLaunchConfig()->SameServer(oldConfig))
	{
		passwdEnc.clear();
		passwdLength = -1;
	}

	// Get local .feb file name
	std::string localFile = FSDir::toAbsolutePath(job->GetFileName());

	QFileInfo info(localFile.c_str());
	std::string baseName = info.baseName().toStdString();

	// Get remote base file name
	remoteFileBase = job->GetLaunchConfig()->remoteDir + "/" + baseName;
}


void CSSHHandler::StartRemoteJob()
{
	// Get local .feb file name
	std::string localFile = FSDir::toAbsolutePath(job->GetFileName());

	// Get remote .feb file name
	std::string remoteFile = remoteFileBase + ".feb";

	// Send .feb file
	if(SendFile(localFile, remoteFile) != SSH_OK)
	{
		EndSSHSession();
		return;
	}

	if(job->GetLaunchConfig()->type == REMOTE)
	{
		// Construct remote command
		std::string command = job->GetLaunchConfig()->path + " " + remoteFile;

		// Run the commands
		if(RunCommand(command) != SSH_OK)
		{
			EndSSHSession();
			return;
		}
	}
	else if(job->GetLaunchConfig()->type == PBS)
	{
		// Create bash file for PBS queueing system
		if(CreateBashFile() != SSH_OK)
		{
			EndSSHSession();
			return;
		}

		// Construct remote command
		std::string command = "qsub " + remoteFileBase + ".bash";

		if(RunInteractiveNoRead(command) != SSH_OK)
		{
			EndSSHSession();
			return;
		}
	}
	else if(job->GetLaunchConfig()->type == SLURM)
	{
		// Create bash file for PBS queueing system
		if(CreateBashFile() != SSH_OK)
		{
			EndSSHSession();
			return;
		}

		// Construct remote command
		std::string command = "sbatch " + remoteFileBase + ".bash";

		if(RunInteractiveNoRead(command) != SSH_OK)
		{
			EndSSHSession();
			return;
		}
	}

	// Close ssh session
	EndSSHSession();
}

void CSSHHandler::GetJobFiles()
{
	// Get local .xplt file name
	std::string localFile = FSDir::toAbsolutePath(job->GetPlotFileName());

	// Get remote .xplt file name
	std::string remoteFile = remoteFileBase + ".xplt";

	// Get .xplt file
	GetFile(localFile, remoteFile);

	// Get local .log file name
	localFile.replace(localFile.end()-4, localFile.end(), "log");

	// Get remote .log file name
	remoteFile = remoteFileBase + ".log";

	// Get .log file
	GetFile(localFile, remoteFile);

	// Close ssh session
	EndSSHSession();
}

void CSSHHandler::GetQueueStatus()
{
	std::string command;

	if(job->GetLaunchConfig()->type == PBS)
	{
		command = "qstat";
	}
	else
	{
		command = "squeue";
	}

	std::vector<std::string> list;
	list.push_back(command);

	RunCommandList(list);

	// Close ssh session
	EndSSHSession();
}

void CSSHHandler::SetPasswordLength(int l)
{
	passwdLength = l;
}
size_t CSSHHandler::GetPasswordLength()
{
	return passwdLength;
}

void CSSHHandler::SetPasswdEnc(std::vector<unsigned char> passwdEnc)
{
	this->passwdEnc = passwdEnc;
}

std::vector<unsigned char>& CSSHHandler::GetPasswdEnc()
{
	return passwdEnc;
}

int CSSHHandler::StartSSHSession()
{
	int error;

	session = ssh_new();
	if(session == NULL)
	{
		QMessageBox::critical(NULL, "FEBio Studio", "Could not initialize SSH session.");
		return FAILED;
	}

	ssh_options_set(session, SSH_OPTIONS_HOST, job->GetLaunchConfig()->server.c_str());
	ssh_options_set(session, SSH_OPTIONS_PORT, &(job->GetLaunchConfig()->port));
	ssh_options_set(session, SSH_OPTIONS_USER, job->GetLaunchConfig()->userName.c_str());

	// Connect to server
	error = ssh_connect(session);
	if (error != SSH_OK)
	{
		QMessageBox::critical(NULL, "FEBio Studio", "Error connecting to server: " + *ssh_get_error(session));
		ssh_free(session);
		return FAILED;
	}

	// Verify the server's identity
	if (verify_knownhost() < 0)
	{
		return FAILED;
	}

	error = authenticatePubkey();
	if(error == OK) return error;

	if(passwdLength != -1)
	{
		if(authenticatePassword()) return OK;
	}

	return NEEDSPSWD;

}

void CSSHHandler::EndSSHSession()
{
	ssh_disconnect(session);
	ssh_free(session);
}

int CSSHHandler::authenticatePubkey()
{
	int rc;

	// Attempt Public Key authentication
	rc = ssh_userauth_publickey_auto(session, NULL, NULL);
	if (rc == SSH_AUTH_SUCCESS)
	{
		return OK;
	}

	return FAILED;
}

bool CSSHHandler::authenticatePassword()
{
	std::string password = CEncrypter::Instance()->Decrypt(passwdEnc, passwdLength);

	int rc = ssh_userauth_password(session, NULL, password.c_str());
	if (rc != SSH_AUTH_SUCCESS) return false;

	return true;
}


int CSSHHandler::verify_knownhost()
{
	enum ssh_known_hosts_e state;
	unsigned char *hash = NULL;
	ssh_key srv_pubkey = NULL;
	size_t hlen;
	char buf[10];
	char *hexa;
	char *p;
	int cmp;
	int rc;

	rc = ssh_get_server_publickey(session, &srv_pubkey);
	if (rc < 0) {
		return -1;
	}

	rc = ssh_get_publickey_hash(srv_pubkey,
			SSH_PUBLICKEY_HASH_SHA1,
			&hash,
			&hlen);
	ssh_key_free(srv_pubkey);
	if (rc < 0) {
		return -1;
	}

	state = ssh_session_is_known_server(session);
	switch (state) {
	case SSH_KNOWN_HOSTS_OK:
		/* OK */
		break;
	case SSH_KNOWN_HOSTS_CHANGED:
		hexa = ssh_get_hexa(hash, hlen);

		QMessageBox::critical(NULL, "FEBio Studio", "Host key for server changed: it is now: " + *hexa);
		//        			+ "\nFor security reasons, connection will be stopped");
		ssh_clean_pubkey_hash(&hash);

		return -1;
	case SSH_KNOWN_HOSTS_OTHER:
		QMessageBox::critical(NULL, "FEBio Studio", "The host key for this server was not found but an other"
				"type of key exists.\n An attacker might change the default server key to confuse your client "
				"into thinking the key does not exist.");
		ssh_clean_pubkey_hash(&hash);

		return -1;
	case SSH_KNOWN_HOSTS_NOT_FOUND:
		//        	mainWindow->AddLogEntry("Could not find known host file.\n "
		//        			"If you accept the host key here, the file will be automatically created.\n");

		/* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */

	case SSH_KNOWN_HOSTS_UNKNOWN:
		hexa = ssh_get_hexa(hash, hlen);
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(NULL, "FEBio Studio", "The server is unknown. Do you trust the host key?\n "
				"Public key hash: " + *hexa, QMessageBox::Yes|QMessageBox::No);
		ssh_string_free_char(hexa);
		ssh_clean_pubkey_hash(&hash);

		if(reply != QMessageBox::Yes)
		{
			return -1;
		}

		rc = ssh_session_update_known_hosts(session);
		if (rc < 0) {
			QMessageBox::critical(NULL, "FEBio Studio", "Error : " + *strerror(errno));
			return -1;
		}

		break;
	case SSH_KNOWN_HOSTS_ERROR:
		QMessageBox::critical(NULL, "FEBio Studio", "Error : " + *strerror(errno));
		ssh_clean_pubkey_hash(&hash);
		return -1;
	}

	ssh_clean_pubkey_hash(&hash);
	return 0;
}

int CSSHHandler::StartSFTPSession()
{
	int rc;
	QString error;

	sftp = sftp_new(session);
	if (sftp == NULL)
	{
		error = QString("Error allocating SFTP session: %1\n").arg(ssh_get_error(session));
		CLogger::AddLogEntry(error);
		return SSH_ERROR;
	}

	rc = sftp_init(sftp);
	if (rc != SSH_OK)
	{
		error = QString("Error initializing SFTP session: %1\n").arg(sftp_get_error(sftp));
		CLogger::AddLogEntry(error);
		sftp_free(sftp);
		return rc;
	}

	return rc;
}

int CSSHHandler::EndSFTPSession()
{
	sftp_free(sftp);
	return SSH_OK;
}

int CSSHHandler::SendFile(std::string local, std::string remote)
{
	int rc, nwritten;
	int access_type = O_WRONLY | O_CREAT | O_TRUNC;
	sftp_file file;
	char buffer[MAX_XFER_BUF_SIZE];
	QString error;

	StartSFTPSession();

	file = sftp_open(sftp, remote.c_str(),
			access_type, S_IRWXU);
	if (file == NULL)
	{
		error = QString("Can't open file for writing: %1\n").arg(ssh_get_error(session));
		emit AddOutput(error);
		return SSH_ERROR;
	}

	ifstream fin(local.c_str(), ios::binary);

	while(fin)
	{
		fin.read(buffer, sizeof(buffer));

		if(fin.gcount() > 0)
		{
			ssize_t nwritten = sftp_write(file, buffer, fin.gcount());
			if (nwritten != fin.gcount())
			{
				error = QString("Can't write data to file: %1\n").arg(ssh_get_error(session));
				emit AddOutput(error);
				sftp_close(file);
				return SSH_ERROR;
			}
		}
	}

	rc = sftp_close(file);
	if (rc != SSH_OK)
	{
		error = QString("Can't close the written file: %1\n").arg(ssh_get_error(session));
		emit AddOutput(error);
		return rc;
	}

	return EndSFTPSession();
}

int CSSHHandler::SendFile(const char * buf, int bufSize, std::string remote)
{
	int rc, nwritten;
	int access_type = O_WRONLY | O_CREAT | O_TRUNC;
	sftp_file file;
	QString error;

	StartSFTPSession();

	file = sftp_open(sftp, remote.c_str(),
			access_type, S_IRWXU);
	if (file == NULL)
	{
		error = QString("Can't open file for writing: %1\n").arg(ssh_get_error(session));
		emit AddOutput(error);
		return SSH_ERROR;
	}

	nwritten = sftp_write(file, buf, bufSize);
	if (nwritten != bufSize)
	{
		error = QString("Can't write data to file: %1\n").arg(ssh_get_error(session));
		emit AddOutput(error);
		sftp_close(file);
		return SSH_ERROR;
	}

	rc = sftp_close(file);
	if (rc != SSH_OK)
	{
		error = QString("Can't close the written file: %1\n").arg(ssh_get_error(session));
		emit AddOutput(error);
		return rc;
	}

	return EndSFTPSession();
}

int CSSHHandler::GetFile(std::string local, std::string remote)
{
	int rc, nbytes;
	int access_type = O_RDWR;
	sftp_file file;
	char buffer[MAX_XFER_BUF_SIZE];
	QString error;

#ifdef WIN32
	HANDLE fileHandle;
	DWORD nwritten;
#else
	int fd, nwritten;
#endif

	StartSFTPSession();

	file = sftp_open(sftp, remote.c_str(), access_type, S_IRWXU);
	if (file == NULL)
	{
		error = QString("Can't open file for writing: %1\n").arg(ssh_get_error(session));
		emit AddOutput(error);
		return SSH_ERROR;
	}

#ifdef WIN32
	fileHandle = CreateFileA(local.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
#else
	fd = open(local.c_str(), O_RDWR | O_CREAT, S_IRWXU);
	if (fd < 0) {
		error = QString("Can't open file for writing: %1\n").arg(strerror(errno));
		emit AddOutput(error);
		return SSH_ERROR;
	}
#endif


	while(true)
	{
		nbytes = sftp_read(file, buffer, sizeof(buffer));

		if (nbytes == 0)
		{
			break; // EOF
		}
		else if (nbytes < 0)
		{
			error = QString("Error while reading file: %1\n").arg(ssh_get_error(session));
			emit AddOutput(error);
			sftp_close(file);

#ifdef WIN32
			CloseHandle(fileHandle);
#endif
			return SSH_ERROR;
		}

#ifdef WIN32
		WriteFile(fileHandle, buffer, nbytes, &nwritten, NULL);
#else
		nwritten = write(fd, buffer, nbytes);

#endif
		if (nwritten != nbytes)
		{
			error = QString("Error writing: %1\n").arg(strerror(errno));
			emit AddOutput(error);
			sftp_close(file);

#ifdef WIN32
			CloseHandle(fileHandle);
#endif
			return SSH_ERROR;
		}
	}

	rc = sftp_close(file);
	if (rc != SSH_OK) {
		error = QString("Can't close the read file: %1\n").arg(ssh_get_error(session));
		emit AddOutput(error);

#ifdef WIN32
		CloseHandle(fileHandle);
#endif

		return rc;
	}

#ifdef WIN32
	CloseHandle(fileHandle);
#endif

	return EndSFTPSession();
}

int CSSHHandler::RunCommand(std::string command)
{
	ssh_channel channel;
	int rc;
	char buffer[256];
	int nbytes;

	channel = ssh_channel_new(session);
	if (channel == NULL)
		return SSH_ERROR;

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		return rc;
	}

	command.push_back('\n');

	rc = ssh_channel_request_exec(channel, command.c_str());
	if (rc != SSH_OK)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return rc;
	}

	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer) - 1, 0);
	while (nbytes > 0)
	{
		buffer[nbytes] = '\0';
		emit AddOutput(buffer);

		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer) - 1, 0);
	}

	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return SSH_ERROR;
	}

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
	return SSH_OK;
}

int CSSHHandler::RunInteractiveNoRead(std::string command)
{
	ssh_channel channel;
	int rc;
	char buffer[256];
	int nbytes;

	channel = ssh_channel_new(session);
	if (channel == NULL)
		return SSH_ERROR;

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		return rc;
	}

	rc = ssh_channel_request_shell(channel);
	if (rc != SSH_OK) return rc;

	command.push_back('\n');

	rc = ssh_channel_write(channel, command.c_str(), sizeof(char)*command.length());

	sleep(1);

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
	return SSH_OK;
}

int CSSHHandler::RunCommandList(std::vector<std::string> commands)
{
	ssh_channel channel, channel2;
	int rc;
	char buffer[256];
	char buffer2[256];
	int nbytes;

	channel = ssh_channel_new(session);
	if (channel == NULL)
		return SSH_ERROR;

	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel);
		return rc;
	}

	channel2 = ssh_channel_new(session);
	if (channel2 == NULL)
		return SSH_ERROR;

	rc = ssh_channel_open_session(channel2);
	if (rc != SSH_OK)
	{
		ssh_channel_free(channel2);
		return rc;
	}

	rc = ssh_channel_request_shell(channel);
	if (rc != SSH_OK) return rc;

	rc = ssh_channel_request_shell(channel2);
	if (rc != SSH_OK) return rc;

	rc = ssh_channel_write(channel, "echo $$\n", sizeof("echo $$\n"));

	nbytes = ssh_channel_read(channel, buffer2, sizeof(buffer2), 0);

	string pidString(buffer2);

	int pid = stoi(pidString);

	for(string command : commands)
	{
		command.push_back('\n');

		rc = ssh_channel_write(channel, command.c_str(), sizeof(char)*command.length());

		bool cont = true;
		while(cont || nbytes > 0)
		{
			usleep(5000);

			string test("pgrep -P ");

			test.append(to_string(pid));
			test.append(" | wc -l\n");

			rc = ssh_channel_write(channel2, test.c_str(), sizeof(char)*test.length());

			ssh_channel_read(channel2, buffer2, sizeof(buffer2), 0);

			string linesString(buffer2);

			int lines = stoi(linesString);

			if(lines < 1) cont = false;

			nbytes = ssh_channel_read_nonblocking(channel, buffer, sizeof(buffer) - 1, 0);
			buffer[nbytes] = '\0';
			emit AddOutput(buffer);
		}
	}

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);

	ssh_channel_send_eof(channel2);
	ssh_channel_close(channel2);
	ssh_channel_free(channel2);
	return SSH_OK;
}


int CSSHHandler::CreateBashFile()
{
	std::string bashString("#!/bin/bash\n\n");

	if(job->GetLaunchConfig()->type == PBS)
	{
		bashString += "#PBS -l nodes=1:ppn=" + std::to_string(job->GetLaunchConfig()->procNum) + "\n\n";
		bashString += "#PBS -l mem=" + std::to_string(job->GetLaunchConfig()->ram) + "\n\n";
		bashString += "#PBS -l walltime=" + job->GetLaunchConfig()->walltime + "\n\n";
		if(!job->GetLaunchConfig()->jobName.empty())
		{
			bashString += "#PBS -N " + job->GetLaunchConfig()->jobName + "\n\n";
			bashString += "#PBS -o " + job->GetLaunchConfig()->remoteDir + "/" + job->GetLaunchConfig()->jobName + "_stdout.log\n\n";
			bashString += "#PBS -e " + job->GetLaunchConfig()->remoteDir + "/" + job->GetLaunchConfig()->jobName + "_stderr.log\n\n";
		}
		bashString += "export OMP_NUM_THREADS=" + std::to_string(job->GetLaunchConfig()->procNum) + "\n";
		bashString += job->GetLaunchConfig()->path + " " + remoteFileBase + ".feb";
	}
	else if(job->GetLaunchConfig()->type == SLURM)
	{
		bashString += "#SBATCH -N 1\n\n";
		bashString += "#SBATCH -n " + std::to_string(job->GetLaunchConfig()->procNum) + "\n\n";
		bashString += "#SBATCH --mem " + std::to_string(job->GetLaunchConfig()->ram) + "\n\n";
		bashString += "#SBATCH -t " + job->GetLaunchConfig()->walltime + "\n\n";
		if(!job->GetLaunchConfig()->jobName.empty())
		{
			bashString += "#SBATCH -J " + job->GetLaunchConfig()->jobName + "\n\n";
			bashString += "#SBATCH -o " + job->GetLaunchConfig()->remoteDir + "/" + job->GetLaunchConfig()->jobName + "_stdout.log\n\n";
			bashString += "#SBATCH -e " + job->GetLaunchConfig()->remoteDir + "/" + job->GetLaunchConfig()->jobName + "_stderr.log\n\n";
		}
		bashString += "export OMP_NUM_THREADS=" + std::to_string(job->GetLaunchConfig()->procNum) + "\n";
		bashString += job->GetLaunchConfig()->path + " " + remoteFileBase + ".feb";
	}

	std::string remote = remoteFileBase + ".bash";

	return SendFile(bashString.c_str(), sizeof(char)*bashString.length(), remote);

}





#endif
