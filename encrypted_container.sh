if [ -f "/home/$USER/secure_data-$USER" ]
    then
        echo "good container"
    else
        u="$USER"
        sudo apt-get install cryptsetup
        dd if=/dev/urandom bs=1M count=50 of=/home/$u/secure_data-$u
        sudo cryptsetup luksFormat /home/$u/secure_data-$u
        sudo cryptsetup luksOpen /home/$u/secure_data-$u secure_data-$u
        sudo mkfs.ext4 /dev/mapper/secure_data-$u
        mkdir /home/$u/secure_data-rw
        echo "/dev/mapper/secure_data-$u /home/$u/secure_data-rw ext4 defaults, noauto 0 0" | sudo tee --append /etc/fstab
    fi
