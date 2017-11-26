if [ -f "/home/$USER/secure_data-$USER" ]
    then
        echo "good container"
    else
        u="$USER"
        if (u -ne "root")
        then
          u=/home/$USER
        else
          u=$USER
        fi
        sudo apt-get install cryptsetup
        dd if=/dev/urandom bs=1M count=50 of=/$u/secure_data-$u
        sudo cryptsetup luksFormat /$u/secure_data-$u
        sudo cryptsetup luksOpen /$u/secure_data-$u secure_data-$u
        sudo mkfs.ext4 /dev/mapper/secure_data-$u
        mkdir /$u/secure_data-rw
        echo "/dev/mapper/secure_data-$u /home/$u/secure_data-rw ext4 defaults, noauto 0 0" | sudo tee --append /etc/fstab
    fi
