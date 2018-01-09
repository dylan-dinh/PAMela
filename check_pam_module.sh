if [ -f "/etc/pam.d/common-auth_bkp" ] && [ -f "/etc/pam.d/common-session_bkp" ] && [ -f "/etc/pam.d/common-account_bkp" ]
&& [ -f "/etc/pam.d/common-password_bkp" ] && [ -f "/lib/x86_64-linux-gnu/security/pam_module.so" ]
then
    echo "pam module is installed"
else
    echo "no pam module detected"
fi