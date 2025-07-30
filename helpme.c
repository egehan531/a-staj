cd ~/openbmc         
. setup evb-ast2500   //Machine name    build/conf/local.conf

cd ~/openbmc/meta-aspeed/recipes-kernel/linux
mkdir -p linux-aspeed/mydriver

linux-aspeed/mydriver/mydriver.cfg
CONFIG_MYDRIVER_V1=y         

linux-aspeed/mydriver/mydriver.scc
define KFEATURE_DESCRIPTION "Enable MyDriver v1"
kconf non-hardware mydriver.cfg


meta-aspeed/recipes-kernel/linux/linux-aspeed.inc
KERNEL_FEATURES:append = " mydriver/mydriver.scc" //en alta


bitbake -c cleansstate virtual/kernel   
bitbake obmc-phosphor-image          




chip base 512
line offset 202
gpio number 714



