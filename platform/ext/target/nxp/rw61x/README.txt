For rdrw612, some TF-M core files are replaced by platform specific variants:

* A new partition is added "loader_service" for RW61x platform. Partition is
  intend to provide services to load any SB3/Raw Wi-Fi/BLE firmware via using
  the conn_fw service available for rw61x.
  location of partition is:
  tf-m\platform\ext\target\nxp\rdrw61x\partitions\loader_service

* Two new nv counters for ITS are added in platform_nv_counters_ids.h.

* In order to be able to use the platform service from the ITS service, this
  dependency has te explicitly be allowed. This is done by altering the
  manifest file tfm_internal_trusted_storage.yaml. From this file, c source
  files are generated, the one file where the change has an effect is
  load_info_tfm_internal_trusted_storage.c. It is checked in as well.

* A check in the platform service restricts usage of certain nv counter values
  for certain services. This needs to be adapted to allow usage of the two new
  ITS nv counters by the ITS service. This is done in platform_sp.c.

* The implementation of the nv counters is changed to use the ocotp of rdrw61x.
  This is contained in platform_sp.c

* S50 is used for built-in TF-M keys. 
  This change is affecting crypto_keys.c and introduced a definition of iak_pk


The idea was to replicate the original TF-M folder structure in here, however
this leads to paths that are too long for IAR to deal with, so all the modified
source files are in this directory. The following list shows their respective
location in the original TF-M folder structure:

* platform_sp.c:
  tf-m\secure_fw\partitions\platform\platform_sp.c

* tfm_internal_trusted_storage.yaml:
  tf-m\secure_fw\partitions\internal_trusted_storage\tfm_internal_trusted_storage.yaml

* load_info_tfm_internal_trusted_storage.c:
  tf-m\secure_fw\partitions\internal_trusted_storage\auto_generated\load_info_tfm_internal_trusted_storage.c

* nv_counters.c:
  tf-m\platform\ext\common\template\nv_counters.c

* crypto_keys.c:
  tf-m\platform\ext\common\template\crypto_keys.c
