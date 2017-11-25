#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION
#include <security/pam_appl.h>
#include <security/pam_modules.h>

/* PAM entry point for creating session*/
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  printf("%s\n","yo fils de pute");
    return(PAM_IGNORE);
}

/* PAM entry point for session cleanup */
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  printf("%s\n", "good bye fils de pute" );
    return(PAM_IGNORE);
}

  /* PAM entry point for accounting */
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return(PAM_IGNORE);
}

  /* PAM entry point for authentication verification */
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    return(PAM_IGNORE);
}
/*
PAM entry point for setting user credentials (that is, to actually
establish the authenticated user's credentials to the service provider)
*/

int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
          return(PAM_IGNORE);
}

  /* PAM entry point for authentication token (password) changes */
int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_IGNORE);
}
