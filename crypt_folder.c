#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION
#define SECURE_DATA "secure_data-rw"
#define FOLDER "secure_data"
#define ROOT_ID 1
#define GOOD_USER_ID 2
#define BAD_USER 3

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>


int good_file(cons char *user_name)
{
  char    buffer[1024];

  memset(buffer, 0, 1024);
  if (sprintf(buffer, "chown %s:%s -R %s%s", user_name, user_name, FOLDER, user_name) == 1)
    return (1);
  system(buffer);

  return (PAM_SUCCESS);
}

int  check_username(const char *user_name)
{
    if (strcmp(user_name, "root") == 0)
      return (ROOT_ID);
    else if (strcmp(user_name, "dydoux") == 0)
      return (GOOD_USER_ID);

    return (0);
}

int crypt_it(const char *user_name, int do_it)
{
  char    buffer[1024];

  if (do_it == 0)
  {
    memset(buffer, 0, 1024);
    printf("Processing for %s\n", user_name);
    if (sprintf(buffer, "echo root | cryptsetup luksopen /home/%s/%s%s %s%s",
        user_name, FOLDER, user_name, FOLDER, user_name) == 1)
          return (1);
        system(buffer);
    if (sprintf(buffer, "mount -t ext4 /dev/mapper/%s%s /home/%s/%s",
                FOLDER, user_name, user_name, SECURE_DATA) == 1)
          return (1);
        system(buffer);
        chmod(user_name=rw ~/secure_data-rw);
        chown(user_name:user_name -R FOLDER user_name);

    return (good_file(user_name));
  }

  if (do_it == 1)
  {
    memset(buffer, 0, 1024);
    printf("Processing encryption...");
    if (sprintf(buffer, "umount /home/%s/%s", user_name, SECURE_DATA) == 1)
      return (1);
    system(buffer);
    if (sprintf(buffer, "cryptsetup luksClose %s%s", FOLDER, user_name) == 1)
      return (1);
    system(buffer);

    return (good_file(user_name));
  }
}

int begin_crypt(const char *user_name, int user_id, int do_it)
{
  printf("User who's getting encrypted folder %s\n", user_name);
  if (do_it == 0)
    return (crypt_it(user_name, 0));
  else if (do_it == 1)
    return (crypt_it(user_name, 1));
}


/* PAM entry point for creating session*/
extern int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char *user = NULL;
  int         pgu_ret, user_id = 0;
  printf("%s\n","Opening pam session");

  //get user
  pgu_ret = pam_get_user(pamh, &user, NULL);
  if (pgu_ret != PAM_SUCCESS || user == NULL)
  {
    syslog(LOG_ERR, "cannot determine user name");
    return(PAM_AUTH_ERR);
  }

  // check user
  if ((user_id = check_username(user)) == 0)
    return (BAD_USER);

  // start all encryption
    return(begin_crypt(user, user_id, 0));
}

/* PAM entry point for session cleanup */
extern int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  const char *user = NULL;
  int         pgu_ret, user_id = 0;
  printf("%s\n","Closing pam session");

  //get user
  pgu_ret = pam_get_user(pamh, &user, NULL);
  if (pgu_ret != PAM_SUCCESS || user == NULL)
  {
    syslog(LOG_ERR, "cannot determine user name");
    return(PAM_AUTH_ERR);
  }

  // check user
  if ((user_id = check_username(user)) == 0)
    return (BAD_USER);

  // start all encryption
    return(begin_crypt(user, user_id, 1));
}



  /* PAM entry point for authentication verification */
extern int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return (PAM_SUCCESS);
}

/*
PAM entry point for setting user credentials (that is, to actually
establish the authenticated user's credentials to the service provider)
*/
extern int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_SUCCESS);
}


  /* PAM entry point for authentication token (password) changes */
extern int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_SUCCESS);
}

/* PAM entry point for accounting */
extern int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_SUCCESS);
}
