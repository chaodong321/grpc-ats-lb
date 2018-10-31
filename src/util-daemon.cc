#include "util-daemon.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

static volatile sig_atomic_t sigflag = 0;

/**
 * \brief Signal handler used to take the parent process out of stand-by
 */
static void SignalHandlerSigusr1 (int signo)
{
    sigflag = 1;
}

/**
 * \brief Tell the parent process the child is ready
 *
 * \param pid pid of the parent process to signal
 */
static void TellWaitingParent (pid_t pid)
{
    kill(pid, SIGUSR1);
}

/**
 * \brief Set the parent on stand-by until the child is ready
 *
 * \param pid pid of the child process to wait
 */
static void WaitForChild (pid_t pid)
{
    int status;
    //printf("Daemon: Parent waiting for child to be ready...");
    /* Wait until child signals is ready */
    while (sigflag == 0) {
        if (waitpid(pid, &status, WNOHANG)) {
            /* Check if the child is still there, otherwise the parent should exit */
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                exit(0);
            }
        }
        /* sigsuspend(); */
        sleep(1);
    }
}

/**
 * \brief Close stdin, stdout, stderr.Redirect logging info to syslog
 *
 */
static void SetupLogging (void)
{
    /* Redirect stdin, stdout, stderr to /dev/null  */
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0)
        return;
    (void)dup2(fd, 0);
    (void)dup2(fd, 1);
    (void)dup2(fd, 2);
    close(fd);
}

/**
 * \brief Daemonize the process
 *
 */
void Daemonize (void)
{
    pid_t pid, sid;

    /* Register the signal handler */
    signal(SIGUSR1, SignalHandlerSigusr1);

    /** \todo We should check if wie allow more than 1 instance
              to run simultaneously. Maybe change the behaviour
              through conf file */

    /* Creates a new process */
    pid = fork();

    if (pid < 0) {
        /* Fork error */
        //printf(SC_ERR_DAEMON, "Error forking the process");
        exit(0);
    } else if (pid == 0) {
        /* Child continues here */
        char *daemondir;

        umask(022);

        sid = setsid();
        if (sid < 0) {
            //printf(SC_ERR_DAEMON, "Error creating new session\n");
            exit(0);
        }

        if (chdir("/") < 0) {
            //printf(SC_ERR_DAEMON, "Error changing to working directory '/'\n");
        }

        SetupLogging();

        /* Child is ready, tell its parent */
        TellWaitingParent(getppid());

        /* Daemon is up and running */
        //printf("Daemon is running\n");
        return;
    }
    /* Parent continues here, waiting for child to be ready */
    //printf("Parent is waiting for child to be ready\n");
    WaitForChild(pid);

    /* Parent exits */
    //printf("Child is ready, parent exiting\n");
    exit(0);

}

