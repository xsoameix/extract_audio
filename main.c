#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  ERR_MEM = 1,
  ERR_IO,
  ERR_UNK_BOX,
  ERR_BOX_SIZE,
  ERR_UNK_HDLR_TYPE,
  ERR_UNK_NAL_UNIT_TYPE,
  ERR_READ_CODE,
  ERR_READ_BIT,
  ERR_READ_BITS,
  ERR_UNK_PIC_ORDER_CNT_TYPE,
  ERR_UNK_OVERSCAN_INFO_PRESENT_FLAG,
  ERR_UNK_CHROMA_LOC_INFO_PRESENT_FLAG,
  ERR_UNK_ASPECT_RATIO_IDC,
  ERR_UNK_NAL_HRD_PARA_PRESENT_FLAG,
  ERR_UNK_VCL_HRD_PARA_PRESENT_FLAG,
  ERR_UNK_NUM_SLICE_GROUPS_MINUS1,
  ERR_UNK_PROFILE_IDC,
  ERR_UNK_TAG,
  ERR_UNK_ES_FLAGS,
  ERR_UNK_ID,
  ERR_EMPTY_BITS,
  ERR_LEN
};

enum {
  TAG_ES_DESCR = 0x03,
  TAG_DECODER_CONFIG_DESCR,
  TAG_DEC_SPECIFIC_INFO,
  TAG_SL_CONFIG_DESCR
};

enum {
  CODEC_AAC = 1
};

#define MKBOX(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

enum {
  BOX_TOP  = MKBOX('t', 'o', 'p', ' '),
  BOX_FTYP = MKBOX('f', 't', 'y', 'p'),
  BOX_MOOV = MKBOX('m', 'o', 'o', 'v'),
  BOX_MVHD = MKBOX('m', 'v', 'h', 'd'),
  BOX_TRAK = MKBOX('t', 'r', 'a', 'k'),
  BOX_TKHD = MKBOX('t', 'k', 'h', 'd'),
  BOX_MDIA = MKBOX('m', 'd', 'i', 'a'),
  BOX_MDHD = MKBOX('m', 'd', 'h', 'd'),
  BOX_HDLR = MKBOX('h', 'd', 'l', 'r'),
  BOX_VIDE = MKBOX('v', 'i', 'd', 'e'),
  BOX_SOUN = MKBOX('s', 'o', 'u', 'n'),
  BOX_MINF = MKBOX('m', 'i', 'n', 'f'),
  BOX_DINF = MKBOX('d', 'i', 'n', 'f'),
  BOX_DREF = MKBOX('d', 'r', 'e', 'f'),
  BOX_URL  = MKBOX('u', 'r', 'l', ' '),
  BOX_URN  = MKBOX('u', 'r', 'n', ' '),
  BOX_STBL = MKBOX('s', 't', 'b', 'l'),
  BOX_STSD = MKBOX('s', 't', 's', 'd'),
  BOX_AVCC = MKBOX('a', 'v', 'c', 'C'),
  BOX_ESDS = MKBOX('e', 's', 'd', 's'),
  BOX_STTS = MKBOX('s', 't', 't', 's'),
  BOX_STSC = MKBOX('s', 't', 's', 'c'),
  BOX_STCO = MKBOX('s', 't', 'c', 'o'),
  BOX_STSZ = MKBOX('s', 't', 's', 'z'),
  BOX_STSS = MKBOX('s', 't', 's', 's'),
  BOX_VMHD = MKBOX('v', 'm', 'h', 'd'),
  BOX_SMHD = MKBOX('s', 'm', 'h', 'd'),
  BOX_MDAT = MKBOX('m', 'd', 'a', 't')
};

union box {
  struct box_top * top;
  struct box_ftyp * ftyp;
  struct box_moov * moov;
  struct box_trak * trak;
  struct box_mdia * mdia;
  struct box_minf * minf;
  struct box_dinf * dinf;
  struct box_stsd * stsd;
  struct box_vide * vide;
  struct box_soun * soun;
};

typedef union box box_t;

struct box_ftyp {
  char m_brand[4]; /* major brand */
  unsigned int m_version; /* minor version */
  char (* c_brands)[4]; /* compatible brands */
};

struct box_mvhd {
  unsigned int c_time; /* creation time */
  unsigned int m_time; /* modification time */
  unsigned int timescale;
  unsigned int duration;
  int rate;
  short volume;
  short _;
  int matrix[9];
  unsigned int next_track_id;
};

struct box_tkhd {
  unsigned char track_enabled;
  unsigned char track_in_movie;
  unsigned char track_in_preview;
  unsigned char _;
  unsigned int c_time;
  unsigned int m_time;
  unsigned int track_id;
  unsigned int duration;
  short layer;
  short alternate_group;
  short volume;
  short _2;
  int matrix[9];
  unsigned int width;
  unsigned int height;
};

struct box_mdhd {
  unsigned int c_time;
  unsigned int m_time;
  unsigned int timescale;
  unsigned int duration;
  char _;
  char lang[3];
};

struct box_hdlr {
  unsigned int type;
  char * name;
};

struct box_dref_entry {
  unsigned int type;
  char _[3];
  unsigned char self_contained;
  char * name;
  char * location;
};

struct box_dref {
  unsigned int entry_count;
  struct box_dref_entry * entry;
};

struct box_dinf {
  struct box_dref dref;
};

struct box_vide {
  unsigned short dref_index;
  unsigned short width;
  unsigned short height;
  unsigned int h_rez; /* horizresolution */
  unsigned int v_rez; /* vertresolution */
  unsigned short frame_count;
  unsigned short depth;
  char compressorname[32];
};

struct box_soun {
  unsigned short dref_index;
  unsigned short channelcount;
  unsigned short samplesize;
  unsigned int samplerate;
};

struct box_stsd {
  unsigned int entry_count;
  box_t entry;
};

struct box_stbl {
  struct box_stsd stsd;
};

struct box_minf {
  struct box_dinf dinf;
  struct box_stbl stbl;
};

struct box_mdia {
  struct box_mdhd mdhd;
  struct box_hdlr hdlr;
  struct box_minf minf;
};

struct box_trak {
  struct box_tkhd tkhd;
  struct box_mdia mdia;
};

struct box_moov {
  struct box_mvhd mvhd;
  struct box_trak * trak;
  unsigned int trak_len;
};

struct box_top {
  struct box_ftyp ftyp;
  struct box_moov moov;
};

struct box_info {
  unsigned int size;
  unsigned int type;
  long pos;
};

typedef int (* box_func)(FILE * file, struct box_info * info, box_t p_box);

struct box_func_pair {
  unsigned int name;
  box_func func;
};

static const char *
box_to_str(unsigned int x) {
  static char s[4];

  s[0] = (char) (x >> 24);
  s[1] = (char) (x >> 16);
  s[2] = (char) (x >> 8);
  s[3] = (char) x;
  return s;
}

static const char *
err_to_str(int i) {
  static const char * errors[] = {
    "",
    "Memory error",
    "IO error",
    "Unknown box",
    "Box size is erroneous",
    "Unknown handler type",
    "Unknown nal unit type",
    "Unable to read exp golomb code",
    "Unable to read a bit",
    "Unable to read bits",
    "Unknown pic order count type",
    "Unknown overscan_info_present_flag",
    "Unknown chroma_loc_info_present_flag",
    "Unknwon aspect_ratio_idc",
    "Unknown nal_hrd_para_present_flag",
    "Unknown vcl_hrd_para_present_flag",
    "Unknown num_slice_groups_minus1",
    "Unknown profile_idc",
    "Unknown tag",
    "Unknown ES flags",
    "Unknown object type indication",
    "Empty bits"
  };
  if (i < 0 || i >= ERR_LEN)
    return NULL;
  return errors[i];
}

static void
indent(int i) {
  static const char blank[] = "          ""          ""          ""          ";
  static int lv = 0;

  if (i == 0)
    printf("%s", blank + strlen(blank) - lv * 2);
  lv += i;
}

static void
attr(const char * name) {
  enum { ATTR_LEN = 25 };
  static char blank[80 + 1] = {0};
  unsigned int i;
  size_t len;

  if (blank[0] == 0) {
    for (i = 0; i < sizeof(blank); i++)
      blank[i] = ' ';
    blank[80] = '\0';
  }

  indent(0);
  len = strlen(name);
  if (len >= ATTR_LEN)
    printf("%s: ", name);
  else if (len >= 1)
    printf("%s:%s ", name, &blank[80-ATTR_LEN+len]);
  else
    printf("%s ", &blank[80-ATTR_LEN-1]);
}

static void
attr_c24(const char * name, const char * c) {
  attr(name); printf("%.3s\n", c);
}

#define ATTR_C24(name) attr_c24(#name, (name));

static void
attr_c32(const char * name, const char * c) {
  attr(name); printf("%.4s\n", c);
}

static void
attr_str(const char * name, const char * str) {
  attr(name); printf("%s\n", str);
}

#define ATTR_STR(name) attr_str(#name, (name))

static void
attr_u(const char * name, unsigned int u) {
  attr(name); printf("%u\n", u);
}

#define ATTR_U(name) attr_u(#name, (name));

static void
attr_s(const char * name, int s) {
  attr(name); printf("%d\n", s);
}

#define ATTR_S(name) attr_s(#name, (name));

static void
attr_s_8(const char * name, int s) {
  attr(name); printf("%d.%x\n", s >> 8, s & 0xff);
}

#define ATTR_S_8(name) attr_s_8(#name, (name));

static void
attr_s_16(const char * name, int s) {
  attr(name); printf("%d.%x\n", s >> 16, s & 0xffff);
}

#define ATTR_S_16(name) attr_s_16(#name, (name));

static void
attr_u_16(const char * name, unsigned int u) {
  attr(name); printf("%u.%x\n", u >> 16, u & 0xffff);
}

#define ATTR_U_16(name) attr_u_16(#name, (name));

static int
mem_alloc(void * ret, size_t size) {
  void ** ret_p;
  void * ptr;

  ptr = malloc(size);
  if (ptr == NULL)
    return ERR_MEM;

  ret_p = ret;
  * ret_p = ptr;
  return 0;
}

static int
mem_realloc(void * ret, size_t size) {
  void ** ret_p;
  void * p;

  ret_p = ret;
  p = realloc(* ret_p, size);
  if (p == NULL)
    return ERR_MEM;

  * ret_p = p;
  return 0;
}

static void
mem_free(void * ptr) {
  free(ptr);
}

static int
skip(FILE * file, long offset) {
  if (fseek(file, offset, SEEK_CUR))
    return ERR_IO;
  return 0;
}

static int
get_pos(long * ret, FILE * file) {
  long pos;

  pos = ftell(file);
  if (pos == -1)
    return ERR_IO;

  * ret = pos;
  return 0;
}

static int
read_ary(void * ptr, size_t size, size_t len, FILE * file) {
  if (fread(ptr, size, len, file) != len)
    return ERR_IO;
  return 0;
}

static int
read_c32(char * ret, FILE * file) {
  if (fread(ret, 4, 1, file) != 1)
    return ERR_IO;
  return 0;
}

static int
read_s16(short * ret, FILE * file) {
  unsigned char b16[2];

  if (fread(b16, sizeof(b16), 1, file) != 1)
    return ERR_IO;

  * ret = (short) ((b16[0] << 8) | b16[1]);
  return 0;
}

static int
read_s32(int * ret, FILE * file) {
  unsigned char b32[4];

  if (fread(b32, sizeof(b32), 1, file) != 1)
    return ERR_IO;

  * ret = (int) ((b32[0] << 24) | (b32[1] << 16) |
                 (b32[2] << 8)  | b32[3]);
  return 0;
}

static int
read_u8(unsigned char * ret, FILE * file) {
  int b8;

  b8 = fgetc(file);
  if (b8 == EOF)
    return ERR_IO;

  * ret = (unsigned char) (b8 & 0xff);
  return 0;
}

static int
read_u16(unsigned short * ret, FILE * file) {
  unsigned char b16[2];

  if (fread(b16, sizeof(b16), 1, file) != 1)
    return ERR_IO;

  * ret = (unsigned short) ((b16[0] << 8) | b16[1]);
  return 0;
}

static int
read_u32(unsigned int * ret, FILE * file) {
  unsigned char b32[4];

  if (fread(b32, sizeof(b32), 1, file) != 1)
    return ERR_IO;

  * ret = (unsigned int) ((b32[0] << 24) | (b32[1] << 16) |
                          (b32[2] << 8)  | b32[3]);
  return 0;
}

static int
read_str(char ** ret, FILE * file) {
  long pos;
  size_t len;
  char * str;
  int err;

  err = 0;

  pos = ftell(file);
  if (pos == -1) {
    err = ERR_IO;
    goto exit;
  }

  len = 0;

  for (;;) {
    int c;

    c = fgetc(file);
    if (c == EOF) {
      err = ERR_IO;
      goto exit;
    }
    if (c == '\0')
      break;

    len++;
  }

  str = malloc(len + 1);
  if (str == NULL) {
    err = ERR_MEM;
    goto exit;
  }

  if (fseek(file, pos, SEEK_SET) == -1) {
    err = ERR_IO;
    goto free;
  }

  if (fread(str, len + 1, 1, file) != 1) {
    err = ERR_IO;
    goto free;
  }

  * ret = str;
free:
  if (err)
    mem_free(str);
exit:
  return err;
}

static int
read_ver(unsigned char * version, unsigned int * flags, FILE * file) {
  unsigned int b32;
  int ret;

  if ((ret = read_u32(&b32, file)) != 0)
    return ret;

  * version = (b32 >> 8) & 0xff;
  * flags = b32 & 0x00ffffff;

#if 0
  attr("version:"); printf("%u\n", * version);
  attr("flags:"); printf("%u\n", * flags);
#endif

  return 0;
}

static int
read_mat(int * matrix, FILE * file) {
  int i;
  int ret;

  for (i = 0; i < 9; i++)
    if ((ret = read_s32(&matrix[i], file)) != 0)
      return ret;

  return 0;
}

static int
read_box(FILE * file, struct box_info * info, box_t box,
         struct box_func_pair * pairs) {
  struct box_info child;
  size_t i;
  int ret;

  for (;;) {
    if ((ret = get_pos(&child.pos, file)) != 0)
      return ret;

    if ((unsigned int) (child.pos - info->pos) > info->size)
      return ERR_BOX_SIZE;
    if ((unsigned int) (child.pos - info->pos) == info->size)
      return 0;

    if ((ret = read_u32(&child.size, file)) != 0 ||
        (ret = read_u32(&child.type, file)) != 0)
      return ret;

    for (i = 0; pairs[i].name; i++)
      if (pairs[i].name == child.type)
        break;

    indent(0); printf("[%.4s %u]\n", box_to_str(child.type), child.size);
    indent(1);

    if (pairs[i].name == 0)
      return ERR_UNK_BOX;

    if ((ret = pairs[i].func(file, &child, box)) != 0)
      return ret;

    indent(-1);
  }
}

static int
read_ftyp(FILE * file, struct box_info * info, box_t p_box) {
  long pos;

  char m_brand[4]; /* major brand */
  unsigned int m_version; /* minor version */
  char (* c_brands)[4]; /* compatible brands */

  struct box_ftyp * ftyp;

  unsigned int i;
  unsigned int len;
  int ret;

  ret = 0;
  c_brands = NULL;

  if ((ret = read_c32(m_brand, file)) != 0 ||
      (ret = read_u32(&m_version, file)) != 0 ||
      (ret = get_pos(&pos, file)) != 0)
    goto exit;

  attr_c32("major_brand", m_brand);
  attr_u("minor_version", m_version);

  len = (info->size - (unsigned int) (pos - info->pos)) / 4;
  if (len > 0) {

    /* compatible brands */
    if ((ret = mem_alloc(&c_brands, len * sizeof(c_brands[0]))) != 0 ||
        (ret = read_ary(c_brands, sizeof(c_brands[0]), len, file)) != 0)
      goto exit;

    for (i = 0; i < len; i++)
      attr_c32("compatible brand", c_brands[i]);
  }
  ftyp = &p_box.top->ftyp;
  memcpy(ftyp->m_brand, m_brand, sizeof(m_brand));
  ftyp->m_version = m_version;
  ftyp->c_brands = c_brands;
exit:
  return ret;
}

static int
read_mvhd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int c_time;
  unsigned int m_time;
  unsigned int timescale;
  unsigned int duration;
  int rate;
  short volume;
  int matrix[9];
  unsigned int next_track_id;
  unsigned int i;
  struct box_mvhd * mvhd;
  int ret;

  (void) info;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&c_time, file)) != 0 ||
      (ret = read_u32(&m_time, file)) != 0 ||
      (ret = read_u32(&timescale, file)) != 0 ||
      (ret = read_u32(&duration, file)) != 0 ||
      (ret = read_s32(&rate, file)) != 0 ||
      (ret = read_s16(&volume, file)) != 0 ||
      (ret = skip(file, 2 + 4 * 2)) != 0 || /* reserved */
      (ret = read_mat(matrix, file)) != 0 ||
      (ret = skip(file, 4 * 6)) != 0 || /* pre defined */
      (ret = read_u32(&next_track_id, file)) != 0)
    return ret;

  attr_u("creation time", c_time);
  attr_u("modification time", m_time);
  ATTR_U(timescale);
  ATTR_U(duration);
  ATTR_S_16(rate);
  ATTR_S_8(volume);

  for (i = 0; i < 3; i++) {
    attr(i == 0 ? "matrix" : "");
    printf("%d.%x %d.%x %d.%x\n",
           matrix[i] >> 16, matrix[i] & 0xffff,
           matrix[i+3] >> 16, matrix[i+3] & 0xffff,
           matrix[i+6] >> 30, matrix[i+6] & 0x3fffffff);
  }

  ATTR_U(next_track_id);

  mvhd = &p_box.moov->mvhd;
  mvhd->c_time = c_time;
  mvhd->m_time = m_time;
  mvhd->timescale = timescale;
  mvhd->duration = duration;
  mvhd->rate = rate;
  mvhd->volume = volume;
  for (i = 0; i < 9; i++)
    mvhd->matrix[i] = matrix[i];
  mvhd->next_track_id = next_track_id;

  return 0;
}

static int
read_tkhd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned char track_enabled;
  unsigned char track_in_movie;
  unsigned char track_in_preview;
  unsigned int c_time;
  unsigned int m_time;
  unsigned int track_id;
  unsigned int duration;
  short layer;
  short alternate_group;
  short volume;
  int matrix[9];
  unsigned int width;
  unsigned int height;
  unsigned int i;
  struct box_tkhd * tkhd;
  int ret;

  (void) info;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&c_time, file)) != 0 ||
      (ret = read_u32(&m_time, file)) != 0 ||
      (ret = read_u32(&track_id, file)) != 0 ||
      (ret = skip(file, 4)) != 0 || /* reserved */
      (ret = read_u32(&duration, file)) != 0 ||
      (ret = skip(file, 4 * 2)) != 0 || /* reserved */
      (ret = read_s16(&layer, file)) != 0 ||
      (ret = read_s16(&alternate_group, file)) != 0 ||
      (ret = read_s16(&volume, file)) != 0 ||
      (ret = skip(file, 2)) != 0 || /* reserved */
      (ret = read_mat(matrix, file)) != 0 ||
      (ret = read_u32(&width, file)) != 0 ||
      (ret = read_u32(&height, file)) != 0)
    return ret;

  track_enabled = flags & 0x000001;
  track_in_movie = (flags & 0x000002) != 0;
  track_in_preview= (flags & 0x000004) != 0;

  ATTR_U(track_enabled);
  ATTR_U(track_in_movie);
  ATTR_U(track_in_preview);
  attr_u("creation time", c_time);
  attr_u("modification time", m_time);
  ATTR_U(track_id);
  ATTR_U(duration);
  ATTR_S(layer);
  ATTR_S(alternate_group);
  ATTR_S_8(volume);

  for (i = 0; i < 3; i++) {
    attr(i == 0 ? "matrix" : "");
    printf("%d.%x %d.%x %d.%x\n",
           matrix[i] >> 16, matrix[i] & 0xffff,
           matrix[i+3] >> 16, matrix[i+3] & 0xffff,
           matrix[i+6] >> 30, matrix[i+6] & 0x3fffffff);
  }

  ATTR_U_16(width);
  ATTR_U_16(height);

  tkhd = &p_box.trak->tkhd;
  tkhd->track_enabled = track_enabled;
  tkhd->track_in_movie = track_in_movie;
  tkhd->track_in_preview = track_in_preview;
  tkhd->c_time = c_time;
  tkhd->m_time = m_time;
  tkhd->track_id = track_id;
  tkhd->duration = duration;
  tkhd->layer = layer;
  tkhd->alternate_group = alternate_group;
  tkhd->volume = volume;
  for (i = 0; i < 9; i++)
    tkhd->matrix[i] = matrix[i];
  tkhd->width = width;
  tkhd->height = height;

  return 0;
}

static int
read_mdhd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int c_time;
  unsigned int m_time;
  unsigned int timescale;
  unsigned int duration;
  unsigned short lang_pack;
  char lang[3];
  struct box_mdhd * mdhd;
  int ret;

  (void) info;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&c_time, file)) != 0 ||
      (ret = read_u32(&m_time, file)) != 0 ||
      (ret = read_u32(&timescale, file)) != 0 ||
      (ret = read_u32(&duration, file)) != 0 ||
      (ret = read_u16(&lang_pack, file)) != 0 ||
      (ret = skip(file, 2)) != 0) /* pre defined */
    return ret;

  lang[0] = (char) ((lang_pack >> 10) + 0x60);
  lang[1] = (char) (((lang_pack >> 5) & 0x1f) + 0x60);
  lang[2] = (char) ((lang_pack & 0x1f) + 0x60);

  attr_u("creation time", c_time);
  attr_u("modification time", m_time);
  ATTR_U(timescale);
  ATTR_U(duration);
  ATTR_C24(lang);

  mdhd = &p_box.mdia->mdhd;
  mdhd->c_time = c_time;
  mdhd->m_time = m_time;
  mdhd->timescale = timescale;
  mdhd->duration = duration;
  memcpy(mdhd->lang, lang, sizeof(lang));

  return 0;
}

static int
read_hdlr(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int type; /* handler type */
  char * name;
  struct box_hdlr * hdlr;
  int ret;

  (void) info;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = skip(file, 4)) != 0 || /* pre defined */
      (ret = read_u32(&type, file)) != 0 ||
      (ret = skip(file, 4 * 3)) != 0 || /* reserved */
      (ret = read_str(&name, file)) != 0)
    return ret;

  attr_c32("handler_type", box_to_str(type));
  ATTR_STR(name);

  hdlr = &p_box.mdia->hdlr;
  hdlr->type = type;
  hdlr->name = name;

  return 0;
}

static int
read_dref_entry(FILE * file, struct box_dref_entry * entry) {
  unsigned int box_size;
  unsigned int box_type;
  unsigned char version;
  unsigned int flags;
  unsigned char self_contained;
  char * name;
  char * location;
  int ret;

  ret = 0;

  if ((ret = read_u32(&box_size, file)) != 0 ||
      (ret = read_u32(&box_type, file)) != 0 ||
      (ret = read_ver(&version, &flags, file)) != 0)
    goto exit;

  indent(0); printf("[%.4s %u]\n", box_to_str(box_type), box_size);

  indent(1);

  self_contained = flags & 0x1;

  ATTR_U(self_contained);

  name = NULL;
  location = NULL;

  if (box_type == BOX_URL) {

    if (self_contained == 0) {

      if ((ret = read_str(&location, file)) != 0)
        goto exit;

      ATTR_STR(location);
    }
  } else if (box_type == BOX_URN) {

    if ((ret = read_str(&name, file)) != 0)
      goto exit;
    if ((ret = read_str(&location, file)) != 0)
      goto free;

    ATTR_STR(name);
    ATTR_STR(location);
free:
    if (ret) {
      mem_free(name);
      goto exit;
    }
  } else {
    ret = ERR_UNK_BOX;
    goto exit;
  }
  entry->type = box_type;
  entry->self_contained = self_contained;
  entry->name = name;
  entry->location = location;
exit:
  indent(-1);
  return ret;
}

static void
free_dref_entry(struct box_dref_entry * entry) {
  mem_free(entry->name);
  mem_free(entry->location);
}

static int
read_dref(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int i;
  unsigned int j;
  struct box_dref_entry * entry;
  struct box_dref * dref;
  int ret;

  (void) info;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    goto exit;

  ATTR_U(entry_count);

  entry = NULL;

  if (entry_count) {

    if ((ret = mem_alloc(&entry, entry_count * sizeof(* entry))) != 0)
      goto exit;

    for (i = 0; i < entry_count; i++) {
      if ((ret = read_dref_entry(file, &entry[i])) != 0)
        goto free;
    }
free:
    if (ret) {
      for (j = 0; j < i; j++)
        free_dref_entry(&entry[j]);
      goto exit;
    }
  }

  dref = &p_box.dinf->dref;
  dref->entry_count = entry_count;
  dref->entry = entry;

exit:
  if (ret)
    mem_free(entry);
  return ret;
}

static int
read_dinf(FILE * file, struct box_info * info, box_t p_box) {
  static struct box_func_pair funcs[] = {
    {BOX_DREF, read_dref},
    {0, NULL}
  };
  box_t box;
  box.dinf = &p_box.mdia->minf.dinf;
  return read_box(file, info, box, funcs);
}

static int
read_vide(FILE * file, struct box_vide * vide) {
  unsigned int box_size;
  unsigned int box_type;
  unsigned short dref_index; /* data reference index */
  unsigned short width;
  unsigned short height;
  unsigned int h_rez;
  unsigned int v_rez;
  unsigned short frame_count;
  unsigned char len;
  char compressorname[32];
  unsigned short depth;
  int ret;

  ret = 0;

  if ((ret = read_u32(&box_size, file)) != 0 ||
      (ret = read_u32(&box_type, file)) != 0 ||
      (ret = skip(file, 6)) != 0 || /* reserved */
      (ret = read_u16(&dref_index, file)) != 0 ||
      (ret = skip(file, 2 + 2 + 4 * 3)) != 0 || /* pre / reserved / pre */
      (ret = read_u16(&width, file)) != 0 ||
      (ret = read_u16(&height, file)) != 0 ||
      (ret = read_u32(&h_rez, file)) != 0 ||
      (ret = read_u32(&v_rez, file)) != 0 ||
      (ret = skip(file, 4)) != 0 || /* reserved */
      (ret = read_u16(&frame_count, file)) != 0 ||
      (ret = read_u8(&len, file)) != 0 ||
      (ret = read_ary(compressorname,
                      sizeof(compressorname) - 1, 1, file)) != 0 ||
      (ret = read_u16(&depth, file)) != 0 ||
      (ret = skip(file, 2)) != 0) /* pre defined */
    goto exit;

  compressorname[len] = '\0';

  indent(0); printf("[%.4s %u]\n", box_to_str(box_type), box_size);

  indent(1);

  ATTR_U(dref_index);
  ATTR_U(width);
  ATTR_U(height);
  ATTR_U_16(h_rez);
  ATTR_U_16(v_rez);
  ATTR_U(frame_count);
  ATTR_STR(compressorname);
  ATTR_U(depth);

  vide->dref_index = dref_index;
  vide->width = width;
  vide->height = height;
  vide->h_rez = h_rez;
  vide->v_rez = v_rez;
  vide->frame_count = frame_count;
  memcpy(vide->compressorname, compressorname, sizeof(compressorname));
  vide->depth = depth;
exit:
  indent(-1);
  return ret;
}

static void
free_vide(struct box_vide * vide) {
  (void) vide;
}

static int
read_soun(FILE * file, struct box_soun * soun) {
  unsigned int box_size;
  unsigned int box_type;
  unsigned short dref_index; /* data reference index */
  unsigned short channelcount;
  unsigned short samplesize;
  unsigned int samplerate;
  int ret;

  ret = 0;

  if ((ret = read_u32(&box_size, file)) != 0 ||
      (ret = read_u32(&box_type, file)) != 0 ||
      (ret = skip(file, 6)) != 0 || /* reserved */
      (ret = read_u16(&dref_index, file)) != 0 ||
      (ret = skip(file, 4 * 2)) != 0 || /* reserved */
      (ret = read_u16(&channelcount, file)) != 0 ||
      (ret = read_u16(&samplesize, file)) != 0 ||
      (ret = skip(file, 2 + 2)) != 0 || /* pre defined / reserved */
      (ret = read_u32(&samplerate, file)) != 0)
    goto exit;

  indent(0); printf("[%.4s %u]\n", box_to_str(box_type), box_size);

  indent(1);

  ATTR_U(dref_index);
  ATTR_U(channelcount);
  ATTR_U(samplesize);
  ATTR_U_16(samplerate);

  soun->dref_index = dref_index;
  soun->channelcount = channelcount;
  soun->samplesize = samplesize;
  soun->samplerate = samplerate;
exit:
  indent(-1);
  return ret;
}

static void
free_soun(struct box_soun * soun) {
  (void) soun;
}

struct bits {
  unsigned char * bytes;
  unsigned int size;
  unsigned int i; /* index of unread bytes */
  unsigned int pos;
  unsigned int buf;
};

static int
init_bits(struct bits * bits, unsigned char * bytes, unsigned int size) {
  if (size == 0)
    return ERR_EMPTY_BITS;
  bits->size = size;
  bits->bytes = bytes;
  bits->buf = bytes[0];
  bits->i = 1;
  bits->pos = 0;
  return 0;
}

static int
read_code(unsigned int * ret, struct bits * bits) {
  unsigned char * b; /* bytes */
  unsigned int size; /* size */
  unsigned int i; /* byte index */
  unsigned int pos; /* bit position */

  unsigned int z; /* number of zeros */
  unsigned int u; /* unsigned byte = byte buf */
  signed int s; /* signed byte */

  unsigned int l; /* last n bits */

  b    = bits->bytes;
  size = bits->size;
  i    = bits->i;
  pos  = bits->pos;
  u    = bits->buf;

  /* read zeros */
  z = 0;
  u &= (unsigned int) 0xff >> pos;
  if (u == 0) { /* the first leading '1' (bit) is not in this byte */
    z += 8 - pos;
    pos = 0;
    for (;;) { /* read next byte to find the first leading '1' */
      if (i >= size)
        return ERR_READ_CODE; /* out of bound */

      u = b[i++];
      if (u)
        break; /* the first leading '1' is found in this byte */
      z += 8;
    }
  }
  s = (int) u << pos;
  while ((signed char) s > 0) { /* read & count zeros (bit) */
    s <<= 1;
    z++;
    pos++;
  }

  if (z > 32)
    return ERR_READ_CODE; /* exceed the maximum of unsigned int */

  /* first non-zero bit */
  pos++;

  if (i + ((pos + z - 1) >> 3) > size)
    return ERR_READ_CODE; /* out of bound */

  /* last n bit */
  if (z > 8 - pos) {
    l = u & ((unsigned int) 0xff >> pos);
    pos = z - (8 - pos);
    for (; pos > 8; pos -= 8) {
      u = b[i++];
      l <<= 8;
      l |= u;
    }
    if (pos) {
      u = b[i++];
      l <<= pos;
      l |= u >> (8 - pos);
    }
  } else {
    l = ((u << pos) & 0xff) >> (8 - z);
    pos += z;
  }

  if (z == 32 && l)
    return ERR_READ_CODE; /* exceed the maximum of unsigned int */

  if (z == 32)
    l = 0xffffffff;
  else
    l += ~(0xffffffff << z);

  bits->buf = u;
  bits->pos = pos;
  bits->i = i;
  * ret = l;

  return 0;
}

static int
read_code_8(unsigned char * c, struct bits * bits) {
  unsigned int n;
  int ret;

  if ((ret = read_code(&n, bits)) != 0)
    return ret;

  if (n >> 8)
    return ERR_READ_CODE;

  * c = n & 0xff;
  return 0;
}

static int
read_bit(unsigned char * ret, struct bits * bits) {
  unsigned char * b;
  unsigned int size;
  unsigned int i;
  unsigned int pos;
  unsigned int buf;
  unsigned char bit;

  b    = bits->bytes;
  size = bits->size;
  i    = bits->i;
  pos  = bits->pos;
  buf  = bits->buf;

  if (pos == 8) {
    if (i >= size)
      return ERR_READ_BIT;

    buf = b[i++];
    pos = 0;
  }

  bit = (buf & ((unsigned int) 0x80 >> pos)) != 0;

  bits->i = i;
  bits->pos = pos + 1;
  bits->buf = buf;
  * ret = bit;

  return 0;
}

static int
read_bits(unsigned int * ret, unsigned char w, /* width */
          struct bits * bits) {
  unsigned char * b;
  unsigned int size;
  unsigned int i;
  unsigned int pos;
  unsigned int u; /* unsigned byte */
  unsigned int r; /* result */

  b    = bits->bytes;
  size = bits->size;
  i    = bits->i;
  pos  = bits->pos;
  u    = bits->buf;

  /* width valid range: 1~32 */

  if (i + ((pos + w - 1) >> 3) > size)
    return ERR_READ_BITS; /* out of bound */

  if (w > 8 - pos) {
    r = u & ((unsigned int) 0xff >> pos);
    pos = w - (8 - pos);
    for (; pos > 8; pos -= 8) {
      u = b[i++];
      r <<= 8;
      r |= u;
    }
    if (pos) {
      u = b[i++];
      r <<= pos;
      r |= u >> (8 - pos);
    }
  } else {
    r = ((u << pos) & 0xff) >> (8 - w);
    pos += w;
  }

  bits->i = i;
  bits->pos = pos;
  bits->buf = u;
  * ret = r;

  return 0;
}

static int
read_bits_8(unsigned char * c, unsigned char w, /* width <= 8 */
            struct bits * bits) {
  unsigned int n;
  int ret;

  if ((ret = read_bits(&n, w, bits)) != 0)
    return ret;

  * c = n & 0xff;
  return 0;
}

/* no need to implement now */
#if 0
static int
read_hrd_para(unsigned int * bi,
              unsigned int * pos,
              unsigned int * buf,
              unsigned int size, unsigned char * bits) {
  unsigned int cpb_cnt_minus1;
#if 0
  unsigned char bit_rate_scale;
  unsigned char cpb_rate_scale;
#endif
  int ret;

  ret = 0;

  if ((ret = read_code(&cpb_cnt_minus1,
                       bi, pos, buf, size, bits)) != 0)
    goto exit;

  attr("cpb_cnt_minus1:"); printf("%u\n", cpb_cnt_minus1);

exit:
  return ret;
}
#endif

static int
read_vui_para(struct bits * bits) {
  unsigned char aspect_ratio_info_present_flag;
  unsigned char aspect_ratio_idc;

  unsigned char overscan_info_present_flag;

  unsigned char video_signal_type_present_flag;
  unsigned char video_format;
  unsigned char video_full_range_flag;

  unsigned char colour_description_present_flag;
  unsigned char colour_primaries;
  unsigned char transfer_characteristics;
  unsigned char matrix_coefficients;

  unsigned char chroma_loc_info_present_flag;

  unsigned char timing_info_present_flag;
  unsigned int num_units_in_tick;
  unsigned int time_scale;
  unsigned char fixed_frame_rate_flag;

  unsigned char nal_hrd_para_present_flag;
  unsigned char vcl_hrd_para_present_flag;
  unsigned char pic_struct_present_flag;

  unsigned char bitstream_restriction_flag;
  unsigned char motion_vectors_over_pic_boundaries_flag;
  unsigned int max_bytes_per_pic_denom;
  unsigned int max_bits_per_mb_denom;
  unsigned int log2_max_mv_length_horizontal;
  unsigned int log2_max_mv_length_vertical;
  unsigned int num_reorder_frames;
  unsigned int max_dec_frame_buffering;

  int ret;

  ret = 0;

  if ((ret = read_bit(&aspect_ratio_info_present_flag, bits)) != 0)
    goto exit;

  ATTR_U(aspect_ratio_info_present_flag);

  if (aspect_ratio_info_present_flag) {
    if ((ret = read_bits_8(&aspect_ratio_idc, 8, bits)) != 0)
      goto exit;

    indent(1);
    ATTR_U(aspect_ratio_idc);
    indent(-1);

    if (aspect_ratio_idc == 0xff) {
      ret = ERR_UNK_ASPECT_RATIO_IDC;
      goto exit;
    }
  }

  if ((ret = read_bit(&overscan_info_present_flag, bits)) != 0)
    goto exit;

  ATTR_U(overscan_info_present_flag);

  if (overscan_info_present_flag) {
    ret = ERR_UNK_OVERSCAN_INFO_PRESENT_FLAG;
    goto exit;
  }

  if ((ret = read_bit(&video_signal_type_present_flag, bits)) != 0)
    goto exit;

  ATTR_U(video_signal_type_present_flag);

  if (video_signal_type_present_flag) {
    if ((ret = read_bits_8(&video_format, 3, bits)) != 0 ||
        (ret = read_bit(&video_full_range_flag, bits)) != 0 ||
        (ret = read_bit(&colour_description_present_flag, bits)) != 0)
      goto exit;

    indent(1);
    ATTR_U(video_format);
    ATTR_U(video_full_range_flag);
    ATTR_U(colour_description_present_flag);

    if (colour_description_present_flag) {
      if ((ret = read_bits_8(&colour_primaries, 8, bits)) != 0 ||
          (ret = read_bits_8(&transfer_characteristics, 8, bits)) != 0 ||
          (ret = read_bits_8(&matrix_coefficients, 8, bits)) != 0)
        goto exit;

      indent(1);
      ATTR_U(colour_primaries);
      ATTR_U(transfer_characteristics);
      ATTR_U(matrix_coefficients);
      indent(-1);
    }
    indent(-1);
  }

  if ((ret = read_bit(&chroma_loc_info_present_flag, bits)) != 0)
    goto exit;

  ATTR_U(chroma_loc_info_present_flag);

  if (chroma_loc_info_present_flag) {
    ret = ERR_UNK_CHROMA_LOC_INFO_PRESENT_FLAG;
    goto exit;
  }

  if ((ret = read_bit(&timing_info_present_flag, bits)) != 0)
    goto exit;

  ATTR_U(timing_info_present_flag);

  if (timing_info_present_flag) {
    if ((ret = read_bits(&num_units_in_tick, 32, bits)) != 0 ||
        (ret = read_bits(&time_scale, 32, bits)) != 0 ||
        (ret = read_bit(&fixed_frame_rate_flag, bits)) != 0)
      goto exit;

    indent(1);
    ATTR_U(num_units_in_tick);
    ATTR_U(time_scale);
    ATTR_U(fixed_frame_rate_flag);
    indent(-1);
  }

  if ((ret = read_bit(&nal_hrd_para_present_flag, bits)) != 0)
    goto exit;

  ATTR_U(nal_hrd_para_present_flag);

  if (nal_hrd_para_present_flag) {
    ret = ERR_UNK_NAL_HRD_PARA_PRESENT_FLAG;
    goto exit;
  }

  if ((ret = read_bit(&vcl_hrd_para_present_flag, bits)) != 0)
    goto exit;

  ATTR_U(vcl_hrd_para_present_flag);

  if (vcl_hrd_para_present_flag) {
    ret = ERR_UNK_VCL_HRD_PARA_PRESENT_FLAG;
    goto exit;
  }
  if (nal_hrd_para_present_flag ||
      vcl_hrd_para_present_flag) {
    ret = ERR_UNK_VCL_HRD_PARA_PRESENT_FLAG;
    goto exit;
  }

  if ((ret = read_bit(&pic_struct_present_flag, bits)) != 0 ||
      (ret = read_bit(&bitstream_restriction_flag, bits)) != 0)
    goto exit;

  ATTR_U(pic_struct_present_flag);
  ATTR_U(bitstream_restriction_flag);

  if (bitstream_restriction_flag) {
    if ((ret = read_bit(&motion_vectors_over_pic_boundaries_flag, bits)) != 0 ||
        (ret = read_code(&max_bytes_per_pic_denom, bits)) != 0 ||
        (ret = read_code(&max_bits_per_mb_denom, bits)) != 0 ||
        (ret = read_code(&log2_max_mv_length_horizontal, bits)) != 0 ||
        (ret = read_code(&log2_max_mv_length_vertical, bits)) != 0 ||
        (ret = read_code(&num_reorder_frames, bits)) != 0 ||
        (ret = read_code(&max_dec_frame_buffering, bits)) != 0)
      goto exit;

    indent(1);
    ATTR_U(motion_vectors_over_pic_boundaries_flag);
    ATTR_U(max_bytes_per_pic_denom);
    ATTR_U(max_bits_per_mb_denom);
    ATTR_U(log2_max_mv_length_horizontal);
    ATTR_U(log2_max_mv_length_vertical);
    ATTR_U(num_reorder_frames);
    ATTR_U(max_dec_frame_buffering);
    indent(-1);
  }
exit:
  return ret;
}

static int
read_nalu(unsigned int size /* size of NALU */, FILE * file) {
  unsigned char ref_idc;
  unsigned char type;
  unsigned int rbsp_size;
  unsigned char * nalu;
  unsigned char * rbsp;
  struct bits bits;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = mem_alloc(&nalu, size)) != 0)
    goto exit;

  if ((ret = read_ary(nalu, size, 1, file)) != 0)
    goto free;

  ref_idc = (unsigned char) ((nalu[0] >> 5) & 0x3);
  type = (unsigned char) (nalu[0] & 0x1f);

  attr_u("nal_ref_idc", ref_idc);
  attr_u("nal_unit_type", type);

  rbsp = nalu;
  rbsp_size = 0;
  for (i = 1; i < size;) {
    if (i + 2 < size && !nalu[i] && !nalu[i+1] && nalu[i+2] == 0x03) {
      rbsp[rbsp_size++] = nalu[i++];
      rbsp[rbsp_size++] = nalu[i++];
      i++; /* emulation_prevention_tree_byte == 0x03 */
    } else {
      rbsp[rbsp_size++] = nalu[i++];
    }
  }

  if (type == 0x7) {
    unsigned char profile_idc; /* AVC profile indication */
    unsigned char profile_comp; /* profile compatibility */
    unsigned char c_set0_flag; /* constraint set 0 flag */
    unsigned char c_set1_flag; /* constraint set 1 flag */
    unsigned char c_set2_flag; /* constraint set 2 flag */
    unsigned char level_idc; /* AVC level indication */
    unsigned char seq_para_set_id; /* seq parameter set id */
    unsigned char log2_max_frame_num_minus4; /* MaxFrameNum used in frame_num */
    unsigned char pic_order_cnt_type; /* to decode picture order count */
    unsigned int num_ref_frames;
    unsigned char gaps_in_frame_num_value_allowed_flag;
    unsigned int pic_width_in_mbs_minus_1;
    unsigned int pic_height_in_mbs_minus_1;
    unsigned char frame_mbs_only_flag;
    unsigned char mb_adaptive_frame_field_flag;
    unsigned char direct_8x8_inference_flag;
    unsigned char frame_cropping_flag;
    unsigned int frame_crop_left_offset;
    unsigned int frame_crop_right_offset;
    unsigned int frame_crop_top_offset;
    unsigned int frame_crop_bottom_offset;
    unsigned char vui_para_present_flag;

    if ((ret = init_bits(&bits, rbsp, rbsp_size)) != 0 ||
        (ret = read_bits_8(&profile_idc, 8, &bits)) != 0 ||
        (ret = read_bits_8(&profile_comp, 8, &bits)) != 0 ||
        (ret = read_bits_8(&level_idc, 8, &bits)) != 0 ||
        (ret = read_code_8(&seq_para_set_id, &bits)) != 0 ||
        (ret = read_code_8(&log2_max_frame_num_minus4, &bits)) != 0 ||
        (ret = read_code_8(&pic_order_cnt_type, &bits)) != 0)
      goto free;

    c_set0_flag = (unsigned char) ((profile_comp >> 7) & 0x1);
    c_set1_flag = (unsigned char) ((profile_comp >> 6) & 0x1);
    c_set2_flag = (unsigned char) ((profile_comp >> 5) & 0x1);

    ATTR_U(profile_idc);
    attr_u("constraint_set0_flag", c_set0_flag);
    attr_u("constraint_set1_flag", c_set1_flag);
    attr_u("constraint_set2_flag", c_set2_flag);
    ATTR_U(level_idc);
    ATTR_U(seq_para_set_id);
    ATTR_U(log2_max_frame_num_minus4);
    ATTR_U(pic_order_cnt_type);

    if (pic_order_cnt_type == 2) {
    } else {
      ret = ERR_UNK_PIC_ORDER_CNT_TYPE;
      goto free;
    }

    if ((ret = read_code(&num_ref_frames, &bits)) != 0 ||
        (ret = read_bit(&gaps_in_frame_num_value_allowed_flag, &bits)) != 0 ||
        (ret = read_code(&pic_width_in_mbs_minus_1, &bits)) != 0 ||
        (ret = read_code(&pic_height_in_mbs_minus_1, &bits)) != 0 ||
        (ret = read_bit(&frame_mbs_only_flag, &bits)) != 0)
      goto free;

    ATTR_U(num_ref_frames);
    ATTR_U(gaps_in_frame_num_value_allowed_flag);
    ATTR_U(pic_width_in_mbs_minus_1);
    ATTR_U(pic_height_in_mbs_minus_1);
    ATTR_U(frame_mbs_only_flag);

    if (!frame_mbs_only_flag) {
      if ((ret = read_bit(&mb_adaptive_frame_field_flag, &bits)) != 0)
        goto free;

      ATTR_U(mb_adaptive_frame_field_flag);
    }

    if ((ret = read_bit(&direct_8x8_inference_flag, &bits)) != 0 ||
        (ret = read_bit(&frame_cropping_flag, &bits)) != 0)
      goto free;

    ATTR_U(direct_8x8_inference_flag);
    ATTR_U(frame_cropping_flag);

    if (frame_cropping_flag) {
      if ((ret = read_code(&frame_crop_left_offset, &bits)) != 0 ||
          (ret = read_code(&frame_crop_right_offset, &bits)) != 0 ||
          (ret = read_code(&frame_crop_top_offset, &bits)) != 0 ||
          (ret = read_code(&frame_crop_bottom_offset, &bits)) != 0)
        goto free;

      ATTR_U(frame_crop_left_offset);
      ATTR_U(frame_crop_right_offset);
      ATTR_U(frame_crop_top_offset);
      ATTR_U(frame_crop_bottom_offset);
    }

    if ((ret = read_bit(&vui_para_present_flag, &bits)) != 0)
      goto free;
    ATTR_U(vui_para_present_flag);

    if (vui_para_present_flag) {
      indent(1);
      if ((ret = read_vui_para(&bits)) != 0)
        goto free;
      indent(-1);
    }
  } else if (type == 0x8) {
    unsigned int pic_para_set_id;
    unsigned int seq_para_set_id;
    unsigned char entropy_coding_mode_flag;
    unsigned char pic_order_present_flag;
    unsigned int num_slice_groups_minus1;
    unsigned int num_ref_idx_10_active_minus1;
    unsigned int num_ref_idx_11_active_minus1;
    unsigned char weighted_pred_flag;
    unsigned int weighted_bipred_idc;
    unsigned int pic_init_qp_minus26;
    unsigned int pic_init_qs_minus26;
    unsigned int chroma_qp_index_offset;
    unsigned char deblocking_filter_control_present_flag;
    unsigned char constrained_intra_pred_flag;
    unsigned char redundant_pic_cnt_present_flag;

    if ((ret = init_bits(&bits, rbsp, rbsp_size)) != 0 ||
        (ret = read_code(&pic_para_set_id, &bits)) != 0 ||
        (ret = read_code(&seq_para_set_id, &bits)) != 0 ||
        (ret = read_bit(&entropy_coding_mode_flag, &bits)) != 0 ||
        (ret = read_bit(&pic_order_present_flag, &bits)) != 0 ||
        (ret = read_code(&num_slice_groups_minus1, &bits)) != 0)
      goto free;

    ATTR_U(pic_para_set_id);
    ATTR_U(seq_para_set_id);
    ATTR_U(entropy_coding_mode_flag);
    ATTR_U(pic_order_present_flag);
    ATTR_U(num_slice_groups_minus1);

    if (num_slice_groups_minus1 > 0) {
      ret = ERR_UNK_NUM_SLICE_GROUPS_MINUS1;
      goto free;
    }

    if ((ret = read_code(&num_ref_idx_10_active_minus1, &bits)) != 0 ||
        (ret = read_code(&num_ref_idx_11_active_minus1, &bits)) != 0 ||
        (ret = read_bit(&weighted_pred_flag, &bits)) != 0 ||
        (ret = read_bits(&weighted_bipred_idc, 2, &bits)) != 0 ||
        (ret = read_code(&pic_init_qp_minus26, &bits)) != 0 ||
        (ret = read_code(&pic_init_qs_minus26, &bits)) != 0 ||
        (ret = read_code(&chroma_qp_index_offset, &bits)) != 0 ||
        (ret = read_bit(&deblocking_filter_control_present_flag, &bits)) != 0 ||
        (ret = read_bit(&constrained_intra_pred_flag, &bits)) != 0 ||
        (ret = read_bit(&redundant_pic_cnt_present_flag, &bits)) != 0)
      goto free;

    ATTR_U(num_ref_idx_10_active_minus1);
    ATTR_U(num_ref_idx_11_active_minus1);
    ATTR_U(weighted_pred_flag);
    ATTR_U(weighted_bipred_idc);
    ATTR_U(pic_init_qp_minus26);
    ATTR_U(pic_init_qs_minus26);
    ATTR_U(chroma_qp_index_offset);
    ATTR_U(deblocking_filter_control_present_flag);
    ATTR_U(constrained_intra_pred_flag);
    ATTR_U(redundant_pic_cnt_present_flag);
  } else {
    ret = ERR_UNK_NAL_UNIT_TYPE;
    goto exit;
  }

free:
  mem_free(rbsp);
exit:
  return ret;
}

static int
read_avcc(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char conf_version; /* configuration version */
  unsigned char profile_idc; /* AVC profile indication */
  unsigned char profile_comp; /* profile compatibility */
  unsigned char c_set0_flag; /* constraint set 0 flag */
  unsigned char c_set1_flag; /* constraint set 1 flag */
  unsigned char c_set2_flag; /* constraint set 2 flag */
  unsigned char level_idc; /* AVC level indication */
  unsigned char len_size_minus_one; /* NALU length size - 1 */

  unsigned char num_of_sps; /* number of sequence parameter sets */
  unsigned short sps_len; /* sequence parameter set length */

  unsigned char num_of_pps; /* number of picture parameter sets */
  unsigned short pps_len; /* picture parameter set length */

/* no need to implement now */
#if 0
  unsigned char chroma_format;
  unsigned char bit_depth_luma_minus8;
  unsigned char bit_depth_chroma_minus8;

  unsigned char num_of_sps_ext; /* number of sequence parameter set ext */
  unsigned short sps_ext_len; /* sequence parameter set ext length */
  unsigned char * sps_ext_nalu; /* sequence parameter set ext NALU */
#endif

  unsigned int i;
  int ret;

  (void) info;
  (void) p_box;

  ret = 0;

  if ((ret = read_u8(&conf_version, file)) != 0 ||
      (ret = read_u8(&profile_idc, file)) != 0 ||
      (ret = read_u8(&profile_comp, file)) != 0 ||
      (ret = read_u8(&level_idc, file)) != 0 ||
      (ret = read_u8(&len_size_minus_one, file)) != 0 ||
      (ret = read_u8(&num_of_sps, file)) != 0)
    goto exit;

  c_set0_flag = (unsigned char) ((profile_comp >> 7) & 0x1);
  c_set1_flag = (unsigned char) ((profile_comp >> 6) & 0x1);
  c_set2_flag = (unsigned char) ((profile_comp >> 5) & 0x1);
  len_size_minus_one = (unsigned char) (len_size_minus_one & 0x3);
  num_of_sps = (unsigned char) (num_of_sps & 0x1f);

  ATTR_U(conf_version);
  ATTR_U(profile_idc);
  attr_u("constraint_set0_flag", c_set0_flag);
  attr_u("constraint_set1_flag", c_set1_flag);
  attr_u("constraint_set2_flag", c_set2_flag);
  ATTR_U(level_idc);
  ATTR_U(len_size_minus_one);
  ATTR_U(num_of_sps);

  indent(1);
  for (i = 0; i < num_of_sps; i++) {
    if ((ret = read_u16(&sps_len, file)) != 0)
      goto exit;

    ATTR_U(sps_len);

    if ((ret = read_nalu(sps_len, file)) != 0)
      goto exit;
  }
  indent(-1);

  if ((ret = read_u8(&num_of_pps, file)) != 0)
    goto exit;

  ATTR_U(num_of_pps);

  indent(1);
  for (i = 0; i < num_of_sps; i++) {
    if ((ret = read_u16(&pps_len, file)) != 0)
      goto exit;

    ATTR_U(pps_len);

    if ((ret = read_nalu(pps_len, file)) != 0)
      goto exit;
  }
  indent(-1);

  if (profile_idc == 100 ||
      profile_idc == 110 ||
      profile_idc == 122 ||
      profile_idc == 144) {
    ret = ERR_UNK_PROFILE_IDC;
    goto exit;
  }
exit:
  return ret;
}

static int
read_tag(unsigned char * ret_tag, unsigned int * ret_len, FILE * file) {
  unsigned char tag;
  unsigned int len;
  unsigned char c;
  int i;
  int ret;

  if ((ret = read_u8(&tag, file)) != 0)
    return ret;

  len = 0;
  for (i = 0; i < 4; i++) {
    if ((ret = read_u8(&c, file)) != 0)
      return ret;
    len <<= 7;
    len |= c & 0x7f;
    if ((c & 0x80) == 0)
      break;
  }
  * ret_len = len;
  * ret_tag = tag;
  return 0;
}

static int
read_id(unsigned char * ret_codec, FILE * file) {
  struct pair {
    unsigned char id;
    unsigned char codec;
  };
  struct pair pairs[] = {
    {0x40, CODEC_AAC} /* Audio ISO/IEC 14496-3 */
  };
  unsigned char id;
  unsigned int i;
  int ret;

  if ((ret = read_u8(&id, file)) != 0)
    return ret;

  attr_u("object_type_idc", id);

  for (i = 0; i < sizeof(pairs)/sizeof(pairs[0]); i++)
    if (pairs[i].id == id) {
      * ret_codec = pairs[i].codec;
      return 0;
    }
  return ERR_UNK_ID;
}

static int
read_esds(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned char tag;
  unsigned int tag_len;

  unsigned short es_id;
  unsigned char es_flags;

  unsigned char codec;
  unsigned char stream_type;
  unsigned char up_stream;
  unsigned int buffer_size_db;
  unsigned int max_bitrate;
  unsigned int avg_bitrate;

  unsigned char * bytes;
  struct bits bits;

  unsigned int audio_object_type;
  unsigned int sampling_frequency_index;
  unsigned int sampling_frequency;
  unsigned int channel_configuration;

  unsigned char predefined;

  int ret;

  (void) info;
  (void) p_box;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_tag(&tag, &tag_len, file)) != 0)
    return ret;

  if (tag != TAG_ES_DESCR)
    return ERR_UNK_TAG;

  indent(0); printf("[ES_Descr %u]\n", tag_len);
  indent(1);

  if ((ret = read_u16(&es_id, file)) != 0 ||
      (ret = read_u8(&es_flags, file)) != 0)
    return ret;

  ATTR_U(es_id);
  ATTR_U(es_flags);

  if (es_flags)
    return ERR_UNK_ES_FLAGS;

  if ((ret = read_tag(&tag, &tag_len, file)) != 0)
    return ret;

  if (tag != TAG_DECODER_CONFIG_DESCR)
    return ERR_UNK_TAG;

  indent(0); printf("[DecoderConfigDescr %u]\n", tag_len);
  indent(1);

  if ((ret = read_id(&codec, file)) != 0 ||
      (ret = read_u32(&buffer_size_db, file)) != 0 ||
      (ret = read_u32(&max_bitrate, file)) != 0 ||
      (ret = read_u32(&avg_bitrate, file)) != 0 ||
      (ret = read_tag(&tag, &tag_len, file)) != 0)
    return ret;

  stream_type = (unsigned char) (buffer_size_db >> 26);
  up_stream = (buffer_size_db >> 25) & 0x1;
  buffer_size_db = buffer_size_db & 0xffffff; 

  ATTR_U(stream_type);
  ATTR_U(up_stream);
  ATTR_U(buffer_size_db);
  ATTR_U(max_bitrate);
  ATTR_U(avg_bitrate);

  if (tag != TAG_DEC_SPECIFIC_INFO ||
      tag_len == 0)
    return ERR_UNK_TAG;

  indent(0); printf("[DecSpecificInfo %u]\n", tag_len);
  indent(1);

  if (codec == CODEC_AAC) {

    if ((ret = mem_alloc(&bytes, tag_len)) != 0)
      return ret;

    if ((ret = read_ary(bytes, tag_len, 1, file)) != 0 ||
        (ret = init_bits(&bits, bytes, tag_len)) != 0 ||
        (ret = read_bits(&audio_object_type, 5, &bits)) != 0)
      goto free;

    if (audio_object_type == 0x1f) {
      if ((ret = read_bits(&audio_object_type, 6, &bits)) != 0)
        goto free;
      audio_object_type += 0x20;
    }

    ATTR_U(audio_object_type);

    if ((ret = read_bits(&sampling_frequency_index, 4, &bits)) != 0)
      goto free;

    ATTR_U(sampling_frequency_index);

    if (sampling_frequency_index == 0xf) {
      if ((ret = read_bits(&sampling_frequency, 24, &bits)) != 0)
        goto free;

      ATTR_U(sampling_frequency);
    } else {
      sampling_frequency = 0;
    }

    if ((ret = read_bits(&channel_configuration, 4, &bits)) != 0)
      goto free;

    ATTR_U(channel_configuration);
free:
    mem_free(bytes);
  }

  indent(-2);

  if ((ret = read_tag(&tag, &tag_len, file)) != 0)
    return ret;

  if (tag != TAG_SL_CONFIG_DESCR)
    return ERR_UNK_TAG;

  indent(0); printf("[SLConfigDescr %u]\n", tag_len);
  indent(1);

  if ((ret = read_u8(&predefined, file)) != 0)
    return ret;

  ATTR_U(predefined);

  indent(-2);

  return ret;
}

static int
read_stsd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int i;
  unsigned int j;
  box_t entry;
  struct box_hdlr * hdlr;
  struct box_stsd * stsd;
  box_t box;
  static struct box_func_pair funcs[] = {
    {BOX_AVCC, read_avcc},
    {BOX_ESDS, read_esds},
    {0, NULL}
  };
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    goto exit;

  ATTR_U(entry_count);

  entry.vide = NULL;

  if (entry_count) {

    hdlr = &p_box.mdia->hdlr;
    if (hdlr->type == BOX_VIDE) {
      struct box_vide * v;

      if ((ret = mem_alloc(&v, entry_count * sizeof(* v))) != 0)
        goto exit;

      for (i = 0; i < entry_count; i++)
        if ((ret = read_vide(file, &v[i])) != 0)
          goto free_vide;

      entry.vide = v;
free_vide:
      if (ret) {
        for (j = 0; j < i; j++)
          free_vide(&v[j]);
        mem_free(v);
      }
    } else if (hdlr->type == BOX_SOUN) {
      struct box_soun * s;

      if ((ret = mem_alloc(&s, entry_count * sizeof(* s))) != 0)
        goto exit;

      for (i = 0; i < entry_count; i++)
        if ((ret = read_soun(file, &s[i])) != 0)
          goto free_soun;

      entry.soun = s;
free_soun:
      if (ret) {
        for (j = 0; j < i; j++)
          free_soun(&s[j]);
        mem_free(s);
      }
    } else {
      ret = ERR_UNK_HDLR_TYPE;
      goto exit;
    }
  }

  box.stsd = &p_box.mdia->minf.stbl.stsd;
  if ((ret = read_box(file, info, box, funcs)) != 0)
    goto free;

  stsd = box.stsd;
  stsd->entry_count = entry_count;
  stsd->entry = entry;
free:
  if (ret) {
    if (hdlr->type == BOX_VIDE) {
      for (i = 0; i < entry_count; i++)
        free_vide(&entry.vide[i]);
      mem_free(entry.vide);
    } else if (hdlr->type == BOX_SOUN) {
      for (i = 0; i < entry_count; i++)
        free_soun(&entry.soun[i]);
      mem_free(entry.soun);
    }
  }
exit:
  return ret;
}

static int
read_stts(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int sample_count;
  unsigned int sample_delta;
  unsigned int i;
  int ret;

  (void) info;
  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    return ret;

  ATTR_U(entry_count);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&sample_count, file)) != 0 ||
        (ret = read_u32(&sample_delta, file)) != 0)
      return ret;

    indent(0); printf("[%u]\n", i);

    indent(1);

    ATTR_U(sample_count);
    ATTR_U(sample_delta);

    indent(-1);
  }
  return 0;
}

static int
read_stsc(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int first_chunk;
  unsigned int samples_per_chunk;
  unsigned int sample_description_index;
  unsigned int i;
  int ret;

  (void) info;
  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    return ret;

  ATTR_U(entry_count);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&first_chunk, file)) != 0 ||
        (ret = read_u32(&samples_per_chunk, file)) != 0 ||
        (ret = read_u32(&sample_description_index, file)) != 0)
      return ret;


    if (entry_count <= 8 ||
        i < 4 || i >= entry_count - 4) {
      indent(0); printf("[%u]\n", i);
      indent(1);

      ATTR_U(first_chunk);
      ATTR_U(samples_per_chunk);
      ATTR_U(sample_description_index);

      indent(-1);
    } else if (i == 4) {
      indent(0); printf("[...]\n");
    }
  }
  return 0;

}

static int
read_stco(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int chunk_offset;
  unsigned int i;
  int ret;

  (void) info;
  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    return ret;

  ATTR_U(entry_count);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&chunk_offset, file)) != 0)
      return ret;

    if (entry_count <= 10 ||
        i < 5 || i >= entry_count - 5) {
      indent(0); printf("[%u] chunk_offset:            "
                        "%u\n", i, chunk_offset);
    } else if (i == 5) {
      indent(0); printf("[...]\n");
    }
  }
  return 0;

}

static int
read_stsz(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int sample_size;
  unsigned int sample_count;
  unsigned int entry_size;
  unsigned int i;
  int ret;

  (void) info;
  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&sample_size, file)) != 0 ||
      (ret = read_u32(&sample_count, file)) != 0)
    return ret;

  ATTR_U(sample_size);
  ATTR_U(sample_count);

  if (sample_size == 0)
    for (i = 0; i < sample_count; i++) {
      if ((ret = read_u32(&entry_size, file)) != 0)
        return ret;

      if (sample_count <= 10 ||
          i < 5 || i >= sample_count - 5) {
        indent(0); printf("[%u] entry_size:            "
                          "%u\n", i, entry_size);
      } else if (i == 5) {
        indent(0); printf("[...]\n");
      }
    }
  return 0;
}

static int
read_stss(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int sample_number;
  unsigned int i;
  int ret;

  (void) info;
  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    return ret;

  ATTR_U(entry_count);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&sample_number, file)) != 0)
      return ret;

    if (entry_count <= 10 ||
        i < 5 || i >= entry_count - 5) {
      indent(0); printf("[%u] sample_number:            "
                        "%u\n", i, sample_number);
    } else if (i == 5) {
      indent(0); printf("[...]\n");
    }
  }
  return 0;
}

static int
read_stbl(FILE * file, struct box_info * info, box_t p_box) {
  static struct box_func_pair funcs[] = {
    {BOX_STSD, read_stsd},
    {BOX_STTS, read_stts},
    {BOX_STSC, read_stsc},
    {BOX_STCO, read_stco},
    {BOX_STSZ, read_stsz},
    {BOX_STSS, read_stss},
    {0, NULL}
  };
  return read_box(file, info, p_box, funcs);
}

static int
read_vmhd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned short graphicsmode;
  unsigned short opcolor[3];
  unsigned int i;
  int ret;

  (void) info;
  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u16(&graphicsmode, file)) != 0)
    return ret;

  ATTR_U(graphicsmode);

  for (i = 0; i < 3; i++)
    if ((ret = read_u16(&opcolor[i], file)) != 0)
      return ret;

  attr_u("opcolor[0]", opcolor[0]);
  attr_u("opcolor[1]", opcolor[1]);
  attr_u("opcolor[2]", opcolor[2]);

  return 0;
}

static int
read_smhd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  short balance;
  int ret;

  (void) info;
  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_s16(&balance, file)) != 0 ||
      (ret = skip(file, 2)) != 0) /* reserved */
    return ret;

  ATTR_S_8(balance);

  return 0;
}

static int
read_minf(FILE * file, struct box_info * info, box_t p_box) {
  static struct box_func_pair funcs[] = {
    {BOX_DINF, read_dinf},
    {BOX_STBL, read_stbl},
    {BOX_VMHD, read_vmhd},
    {BOX_SMHD, read_smhd},
    {0, NULL}
  };
  return read_box(file, info, p_box, funcs);
}

static int
read_mdia(FILE * file, struct box_info * info, box_t p_box) {
  static struct box_func_pair funcs[] = {
    {BOX_MDHD, read_mdhd},
    {BOX_HDLR, read_hdlr},
    {BOX_MINF, read_minf},
    {0, NULL}
  };
  box_t box;
  box.mdia = &p_box.trak->mdia;
  return read_box(file, info, box, funcs);
}

static void
free_mdia(struct box_mdia * mdia) {
  unsigned int i;

  if (mdia->hdlr.type == BOX_VIDE) {
    for (i = 0; i < mdia->minf.stbl.stsd.entry_count; i++)
      free_vide(&mdia->minf.stbl.stsd.entry.vide[i]);
    mem_free(mdia->minf.stbl.stsd.entry.vide);
  } else if (mdia->hdlr.type == BOX_SOUN) {
    for (i = 0; i < mdia->minf.stbl.stsd.entry_count; i++)
      free_soun(&mdia->minf.stbl.stsd.entry.soun[i]);
    mem_free(mdia->minf.stbl.stsd.entry.soun);
  }
  for (i = 0; i < mdia->minf.dinf.dref.entry_count; i++)
    free_dref_entry(&mdia->minf.dinf.dref.entry[i]);
  mem_free(mdia->minf.dinf.dref.entry);
  mem_free(mdia->hdlr.name);
}

static int
read_trak(FILE * file, struct box_info * info, box_t p_box) {
  static struct box_func_pair funcs[] = {
    {BOX_TKHD, read_tkhd},
    {BOX_MDIA, read_mdia},
    {0, NULL}
  };
  struct box_moov * moov;
  box_t box;
  int ret;

  moov = p_box.moov;
  if ((ret = mem_realloc(&moov->trak,
                         (moov->trak_len + 1) * sizeof(* moov->trak))) != 0)
    return ret;

  box.trak = &moov->trak[moov->trak_len];
  if ((ret = read_box(file, info, box, funcs)) != 0)
    return ret;

  moov->trak_len++;
  return 0;
}

static int
read_moov(FILE * file, struct box_info * info, box_t p_box) {
  static struct box_func_pair funcs[] = {
    {BOX_MVHD, read_mvhd},
    {BOX_TRAK, read_trak},
    {0, NULL}
  };
  box_t box;
  box.moov = &p_box.top->moov;
  return read_box(file, info, box, funcs);
}

static int
read_mdat(FILE * file, struct box_info * info, box_t p_box) {
  long pos;
  int ret;

  (void) p_box;

  if ((ret = get_pos(&pos, file)) != 0)
    return ret;

  return skip(file, info->size - (unsigned int) (pos - info->pos));
}

static int
read_file(FILE * file, struct box_info * info, struct box_top * top) {
  box_t box;
  static struct box_func_pair funcs[] = {
    {BOX_FTYP, read_ftyp},
    {BOX_MOOV, read_moov},
    {BOX_MDAT, read_mdat},
    {0, NULL}
  };
  box.top = top;
  return read_box(file, info, box, funcs);
}

static int
open_file(FILE ** file_p, struct box_info * info,
          struct box_top * top, const char * fname) {
  FILE * file;
  long size;
  int ret;

  file = fopen(fname, "rb");
  if (file == NULL) {
    ret = ERR_IO;
    goto exit;
  }

  if (fseek(file, 0, SEEK_END) == -1) {
    ret = ERR_IO;
    goto close;
  }

  if ((ret = get_pos(&size, file)) != 0)
    goto close;

  if (fseek(file, 0, SEEK_SET) == -1) {
    ret = ERR_IO;
    goto close;
  }

  info->pos = 0;
  info->size = (unsigned int) size;
  info->type = BOX_TOP;

  top->ftyp.c_brands = NULL;
  top->moov.trak = NULL;
  top->moov.trak_len = 0;

  * file_p = file;
close:
  if (ret)
    fclose(file);
exit:
  return ret;
}

static void
close_file(FILE * file, struct box_top * top) {
  unsigned int i;

  for (i = 0; i < top->moov.trak_len; i++)
    free_mdia(&top->moov.trak[i].mdia);

  mem_free(top->moov.trak);
  mem_free(top->ftyp.c_brands);

  fclose(file);
}

int
main(int argc, char ** argv) {
  const char * fname;
  FILE * file;
  struct box_info info;
  struct box_top top;
  int ret;

  (void) argc;
  (void) argv;

  /* open file */
  fname = "standbyme.mp4";
  if ((ret = open_file(&file, &info, &top, fname)) != 0)
    goto exit;

  ret = read_file(file, &info, &top);

  close_file(file, &top);
exit:
  if (ret)
    printf("Error: %s\n", err_to_str(ret));
  return ret;
}
