# This file is automatically generated.  DO NOT EDIT!
# Generated from: NetBSD: mknative-gdb,v 1.8 2019/05/29 01:56:06 christos Exp 
# Generated from: NetBSD: mknative.common,v 1.16 2018/04/15 15:13:37 christos Exp 
#
G_libbfd_la_DEPENDENCIES=elf32-vax.lo elf32.lo elf.lo elflink.lo elf-attrs.lo elf-strtab.lo elf-eh-frame.lo dwarf1.lo dwarf2.lo vaxnetbsd.lo aout32.lo vax1knetbsd.lo elf32-gen.lo plugin.lo cpu-vax.lo cpu-plugin.lo netbsd-core.lo  ofiles
G_libbfd_la_OBJECTS=archive.lo archures.lo bfd.lo bfdio.lo bfdwin.lo cache.lo coff-bfd.lo compress.lo corefile.lo elf-properties.lo format.lo hash.lo init.lo libbfd.lo linker.lo merge.lo opncls.lo reloc.lo section.lo simple.lo stab-syms.lo stabs.lo syms.lo targets.lo binary.lo ihex.lo srec.lo tekhex.lo verilog.lo
G_DEFS=-DHAVE_CONFIG_H
G_INCLUDES=
G_TDEFAULTS=-DDEFAULT_VECTOR=vax_elf32_vec -DSELECT_VECS='&vax_elf32_vec,&vax_aout_nbsd_vec,&vax_aout_1knbsd_vec,&elf32_le_vec,&elf32_be_vec,&plugin_vec' -DSELECT_ARCHITECTURES='&bfd_vax_arch,&bfd_plugin_arch'