if [ -f "/home/$USER/secure_data_$USER" ]
    then
        echo "container is ok"
    else
	u="$USER"
        sudo apt-get install cryptsetup
        dd if=/dev/urandom bs=1M count=50 of=/home/$u/secure_data_$u
        sudo cryptsetup luksFormat /home/$u/secure_data_$u
        sudo cryptsetup luksOpen /home/$u/secure_data_$u secure_data_$u
        sudo mkfs.ext4 /dev/mapper/secure_data_$u
        mkdir /home/$u/secure_data-rw
	echo "/dev/mapper/secure_data_$u /home/$u/secure_data-rw ext4 defaults,noauto 0 0" | sudo tee --append /etc/fstab
    fi
