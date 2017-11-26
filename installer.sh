
if [ "./installer.sh" = $0 ]
then
    echo "\nauth optional pam_module.so\n" >> /etc/pam.d/common-auth
    echo "\nsession optional pam_module.so\n" >> /etc/pam.d/common-session
    echo "\naccount optional pam_module.so\n" >> /etc/pam.d/common-account
    echo "\npassword optional pam_module.so\n" >> /etc/pam.d/common-password
    mv installer.sh uninstaller.sh
fi

if [ "./uninstaller.sh" = $0 ]
then
    head -n -3 /etc/pam.d/common-auth > /etc/pam.d/auth; mv /etc/pam.d/auth /etc/pam.d/common-auth
    head -n -3 /etc/pam.d/common-session > /etc/pam.d/session; mv /etc/pam.d/session /etc/pam.d/common-session
    head -n -3 /etc/pam.d/common-account > /etc/pam.d/account; mv /etc/pam.d/account /etc/pam.d/common-account
    head -n -3 /etc/pam.d/common-password > /etc/pam.d/password; mv /etc/pam.d/password /etc/pam.d/common-password
   mv uninstaller.sh installer.sh
fi
