#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <security/pam_appl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CONTAINER "secure_data_"
#define MOUNT "secure_data-rw"

char    cmd[1000];

int	right(const char *user)
{
  memset(cmd, 0, 1000);
  if (sprintf(cmd, "chown %s:%s -R /home/%s/%s%s", user, user, user, CONTAINER ,user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  if (sprintf(cmd, "chmod 700 -R /home/%s/%s%s", user, CONTAINER, user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  if (sprintf(cmd, "chown %s:%s -R /home/%s/%s", user, user, user, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  if (sprintf(cmd, "chmod 700 -R /home/%s/%s", user, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  return (PAM_SUCCESS);
}

int	decipher(const char *user)
{
  memset(cmd, 0, 1000);
  printf("Deciphering \033[32m%s\033[0m container in progress... Please wait a moment\n", user);
  if (sprintf(cmd, "cryptsetup luksOpen /home/%s/%s%s %s%s",
	      user, CONTAINER, user, CONTAINER, user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  if (sprintf(cmd, "mount -t ext4 /dev/mapper/%s%s /home/%s/%s",
	      CONTAINER, user, user, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  return (right(user));  
}

int	cipher(const char *user)
{
  memset(cmd, 0, 1000);
  printf("Ciphering \033[32m%s\033[0m container in progress... Please wait a moment\n", user);
  if (sprintf(cmd, "umount /home/%s/%s", user, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  if (sprintf(cmd, "cryptsetup luksClose %s%s",CONTAINER, user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  return (right(user));
}

int	check_container(const char *user)
{
  memset(cmd, 0, 1000);
  if (sprintf(cmd, "/home/%s/secure_data_%s", user, user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  if (open(cmd, O_RDONLY) == -1)
    {
      printf("Container is ready");
      if (sprintf(cmd, "apt-get install cryptsetup") == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(cmd);
      if (sprintf(cmd, "dd if=/dev/urandom bs=1M count=50 of=/home/%s/secure_data_%s", user, user) == EXIT_FAILURE)
      	return (!PAM_SUCCESS);
      system(cmd);
      if (sprintf(cmd, "cryptsetup luksFormat /home/%s/secure_data_%s", user, user) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(cmd);
      if (sprintf(cmd, "cryptsetup luksOpen /home/%s/secure_data_%s secure_data_%s", user, user, user) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(cmd);
      if (sprintf(cmd, "mkfs.ext4 /dev/mapper/secure_data_%s", user) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(cmd);
      if (sprintf(cmd, "mkdir /home/%s/secure_data-rw", user) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(cmd);
      if (sprintf(cmd, "echo '/dev/mapper/secure_data_%s /home/%s/secure_data-rw ext4 defaults, noauto 0 0' | tee --append /etc/fstab", user, user) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(cmd);
    }
  return (PAM_SUCCESS);
}

extern int	pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char	*user;

  memset(cmd, 0, 1000);
  printf("Opening Session\n");
  if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS)
    return (!PAM_SUCCESS);
  if (strcmp(user, "root") == 0)
    return (PAM_SUCCESS);
  if (check_container(user) != PAM_SUCCESS)
    return (!PAM_SUCCESS);
  return (decipher(user));
}

extern int	pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char *user;

  printf("Closing Session\n");
  if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS)
    return (!PAM_SUCCESS);
  if (strcmp(user, "root") == 0)
    return (PAM_SUCCESS);
  return (cipher(user));
}

void            clean(pam_handle_t *pamh, void *data, int error_status)
{
  char          *xx;

  if ((xx = data))
    while (*xx)
      *xx++ = '\0';
  free (data);
}

extern int	pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return (PAM_SUCCESS);
}

extern int	pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return (PAM_SUCCESS);
}

extern int	pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return (PAM_SUCCESS);
}

extern int	pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return (PAM_SUCCESS);
}
