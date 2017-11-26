#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <security/pam_appl.h>

#define CONTAINER "secure_data_"
#define MOUNT "secure_data-rw"

int		file_right(const char *user)
{
  char		cmd[1024];

  memset(cmd, 0, 1024);
  if (sprintf(cmd, "chown %s:%s -R %s%s", user, user, CONTAINER ,user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  return (PAM_SUCCESS);
}

int		ucrypt(const char *user)
{
  char		cmd[1024];
  int		len;

  memset(cmd, 0, 1024);
  printf("\033[31mDeciphering \033[32m%s\033[31m disk in progress... Please wait a moment\033[0m\n", user);
  if (sprintf(cmd, "echo root | cryptsetup luksOpen /home/%s/%s%s %s%s",
	      user, CONTAINER, user, CONTAINER, user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  if (sprintf(cmd, "mount -t ext4 /dev/mapper/%s%s /home/%s/%s",
	      CONTAINER, user, user, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  return (file_right(user));  
}

int		my_crypt(const char *user)
{
  char		cmd[1024];

  printf("\033[32m%s\033[31m volume encryption... Please wait a moment\033[0m\n", user);
  if (sprintf(cmd, "umount /home/%s/%s", user, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  if (sprintf(cmd, "cryptsetup luksClose %s%s",CONTAINER, user) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(cmd);
  return (PAM_SUCCESS);
}

int		init_crypt(const char *user, int id, int action)
{
  printf("\033[34mUser indentified : \033[32m%s\033[0m\n", user);
  if (action == 0)
    return (ucrypt(user));
  else if (action == 1)
    return (my_crypt(user));
}

int		check_user(const char *user)
{
  if (strcmp(user, "pamela") == 0)
    return (1);
  else if (strcmp(user, "stephan") == 0)
    return (2);
  return (0);
}

extern int	pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char	*user;
  int		type;
  int		ret;

  ret = 0;
  type = 0;
  printf("\033[33mOpening session\033[0m\n");
  if ((ret = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS)
    return (!PAM_SUCCESS);
  if (strcmp(user, "root") == 0)
    return (PAM_SUCCESS);
  if ((type = check_user(user)) == 0)
    return (!PAM_SUCCESS);
  return (init_crypt(user, type, 0));
}

extern int	pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char *user;
  int	type;
  int	ret;

  ret = 0;
  type = 0;
  printf("\033[33mClosing Session...\033[0m\n");
  if ((ret = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS)
    return (ret);
  if (strcmp(user, "root") == 0)
    return (PAM_SUCCESS);
  if ((type = check_user(user)) == 0)
    return (!PAM_SUCCESS);
  return (init_crypt(user, type, 1));
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
