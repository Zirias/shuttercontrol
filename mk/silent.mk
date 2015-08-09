V?= 0

ifeq ($(V),1)
VCC:=
VCXX:=
VAS:=
VDEP:=
VLD:=
VAR:=
VCCLD:=
VMD:=
VMOC:=
VGEN:=
VGENT:=
VR:=
else
VCC=	@echo "   [CC]   $@"
VCXX=	@echo "   [CXX]  $@"
VAS=	@echo "   [AS]   $@"
VDEP=	@echo "   [DEP]  $@"
VLD=	@echo "   [LD]   $@"
VAR=	@echo "   [AR]   $@"
VCCLD=	@echo "   [CCLD] $@"
VMD=	@echo "   [MD]   $@"
VMOC=	@echo "   [MOC]  $@"
VGEN=	@echo "   [GEN]  $@"
VGENT=	@echo "   [GEN]  $@: $(VTAGS)"
VR:=	@
endif

