if [ -f "/home/$USER/secure_data-$USER" ]
    then
        echo "good container"
    else
        u="$USER"
        if (u -ne "root")
        then
          u=home/$USER
        else
          u=$USER
        fi
        sudo apt-get install cryptsetup
        dd if=/dev/zero of=/$u/encrypted.img bs=1 count=0 seek=1G
        dd if=/dev/urandom of=/$u/secure_data-$u.key bs=256 count=1
        sudo cryptsetup luksFormat /$u/encrypted.img /$u/secure_data-$u.key
        sudo cryptsetup luksOpen /$u/encrypted.img $u_volume --key-file /$u/secure_data-$u.key
        sudo mkfs.ext4 /dev/mapper/$u_volume
        mkdir -p /$u/secure_data-rw
        echo "/dev/mapper/secure_data-$u /home/$u/secure_data-rw ext4 defaults, noauto 0 0" | sudo tee --append /etc/fstab
    fi
