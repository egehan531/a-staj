cd /home/ytigli/openbmc/poky
source oe-init-build-env
cd /home/ytigli/openbmc/poky/build

/home/ytigli/openbmc/
├─ meta-aspeed
├─ meta-openbmc-bsp
├─ poky/                  <-- oe‑init‑build‑env’i burada source ettin
│   └─ build/             <-- $BUILDDIR
└─ ...                    <-- diğer meta-* katmanlar

cd /home/ytigli/openbmc/
bitbake-layers create-layer meta-ege
bitbake-layers add-layer meta-ege

cd /home/ytigli/openbmc/meta-ege
mkdir -p recipes-kernel/linux
cd recipes-kernel/linux

nano linux-aspeed_%.bbappend

SRC_URI = "git://github.com/egehan531/mylinux.git;branch=master;protocol=https"
SRCREV  = "${AUTOREV}"
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI += "file://mydriver.cfg"
KCONFIG_MODE = "--alldefconfig"

nano mydriver.cfg

CONFIG_MYDRIVER=y


cd /home/ytigli/openbmc/poky/build
bitbake obmc-phosphor-image



//OFFLINE
git clone https://github.com/egehan531/mylinux.git
tar -czf mylinux.tar.gz mylinux/
sha256sum mylinux.tar.gz
docker cp mylinux.tar.gz <container_id>:/home/ytigli/openbmc/meta-ege/recipes-kernel/linux/

cd /home/ytigli/openbmc/meta-ege/recipes-kernel/linux/
nano linux-aspeed_%.bbappend

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI = "file://mylinux-a1b2c3d.tar.gz"
SRC_URI[sha256sum] = "xxxxxxxx..."   # opsiyonel ama tavsiye
S = "${WORKDIR}/mylinux"             # tar’daki --prefix ile aynı!

cd /home/ytigli/openbmc/poky/build
bitbake -c cleansstate linux-aspeed
bitbake obmc-phosphor-image




/* Tek satırlık SRC_URI değişikliği için bile yeni katman açmaya değmez diyorsan
geçici olarak meta-aspeed/recipes-kernel/linux/linux-aspeed_%.bb dosyasındaki SRC_URI’yı
fork’una çevirip derleyebilirsin; fakat bu, sstate’ler karışır ve sonraki upstream 
çekişlerinde senin değişikliğin kaybolur. Uzun vadede katman + bbappend en temiz yoldur.*/




tree /home/ytigli/openbmc/meta-ege -L 3


meta-ege
└── recipes-kernel
    └── linux
        ├── linux-aspeed_%.bbappend
        └── mydriver.cfg
