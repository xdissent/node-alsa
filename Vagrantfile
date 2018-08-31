
Vagrant.configure("2") do |config|
  config.vm.box = "debian/contrib-testing64"

  config.vm.provider "virtualbox" do |vb|
    vb.customize ["modifyvm", :id, "--usb", "on"]
    vb.customize ["modifyvm", :id, "--usbxhci", "on"]
    vb.customize ["usbfilter", "add", "0", "--target", :id, "--name", "USB Audio", "--vendorid", "0x08bb", "--productid", "0x2902"]
  end

  config.vm.provision "shell", inline: <<~SHELL
    set -e

    wget -qO - https://dl.yarnpkg.com/debian/pubkey.gpg \
      | apt-key add - >/dev/null 2>&1
    echo 'deb http://dl.yarnpkg.com/debian/ stable main' \
      >/etc/apt/sources.list.d/yarn.list
    wget -qO - https://deb.nodesource.com/gpgkey/nodesource.gpg.key \
      | apt-key add - >/dev/null 2>&1
    echo 'deb http://deb.nodesource.com/node_10.x buster main' \
      >/etc/apt/sources.list.d/nodesource.list

    apt-get update
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
      vim \
      git \
      curl \
      'nodejs=10.*' \
      yarn \
      clang-format \
      build-essential \
      libasound2-dev \
      alsa-utils

    su vagrant <<EOF
      cd ~vagrant
      git config --global user.name '#{`git config --global --get user.name`.strip}'
      git config --global user.email '#{`git config --global --get user.email`.strip}'
      git config --global credential.helper store
      git config --global alias.co checkout
      git config --global alias.br branch
      git config --global alias.ci commit
      git config --global alias.st status
    EOF
  SHELL
end
