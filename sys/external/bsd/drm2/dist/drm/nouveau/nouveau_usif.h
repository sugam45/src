/*	$NetBSD: nouveau_usif.h,v 1.3 2018/12/21 07:51:17 maya Exp $	*/

#ifndef __NOUVEAU_USIF_H__
#define __NOUVEAU_USIF_H__

void usif_client_init(struct nouveau_cli *);
void usif_client_fini(struct nouveau_cli *);
#ifdef __NetBSD__
int  usif_ioctl(struct drm_file *, void *, u32);
#else
int  usif_ioctl(struct drm_file *, void __user *, u32);
#endif
int  usif_notify(const void *, u32, const void *, u32);

#endif
