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
  ERR_ENTRY_COUNT,
  ERR_NO_AVCC,
  ERR_UNK_CODEC,
  ERR_STR_LEN,
  ERR_WRITE_BIT,
  ERR_WRITE_BITS,
  ERR_BOX_QTY,
  ERR_NO_SOUN,
  ERR_LEN
};

enum {
  NAL_SPS = 0x7,
  NAL_PPS
};

enum {
  TAG_ES_DESCR = 0x03,
  TAG_DECODER_CONFIG_DESCR,
  TAG_DEC_SPECIFIC_INFO,
  TAG_SL_CONFIG_DESCR,
  TAG_MP4_IOD = 0x10
};

enum {
  CODEC_AAC = 1
};

enum {
  BOX_QTY_0_OR_1 = 1,
  BOX_QTY_0_TO_N = 1 << 1,
  BOX_QTY_1      = 1 << 2,
  BOX_QTY_1_TO_N = 1 << 3
};

#define MKBOX(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

enum {
  BOX_NIL  = MKBOX(' ', ' ', ' ', ' '),
  BOX_TOP  = MKBOX('t', 'o', 'p', ' '),
  BOX_UDTA = MKBOX('u', 'd', 't', 'a'),
  BOX_FTYP = MKBOX('f', 't', 'y', 'p'),
  BOX_MOOV = MKBOX('m', 'o', 'o', 'v'),
  BOX_MVHD = MKBOX('m', 'v', 'h', 'd'),
  BOX_IODS = MKBOX('i', 'o', 'd', 's'),
  BOX_TRAK = MKBOX('t', 'r', 'a', 'k'),
  BOX_TKHD = MKBOX('t', 'k', 'h', 'd'),
  BOX_EDTS = MKBOX('e', 'd', 't', 's'),
  BOX_ELST = MKBOX('e', 'l', 's', 't'),
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
  BOX_AVC1 = MKBOX('a', 'v', 'c', '1'),
  BOX_AVCC = MKBOX('a', 'v', 'c', 'C'),
  BOX_MP4A = MKBOX('m', 'p', '4', 'a'),
  BOX_ESDS = MKBOX('e', 's', 'd', 's'),
  BOX_STTS = MKBOX('s', 't', 't', 's'),
  BOX_CTTS = MKBOX('c', 't', 't', 's'),
  BOX_STSC = MKBOX('s', 't', 's', 'c'),
  BOX_STCO = MKBOX('s', 't', 'c', 'o'),
  BOX_STSZ = MKBOX('s', 't', 's', 'z'),
  BOX_STSS = MKBOX('s', 't', 's', 's'),
  BOX_SGPD = MKBOX('s', 'g', 'p', 'd'),
  BOX_SBGP = MKBOX('s', 'b', 'g', 'p'),
  BOX_VMHD = MKBOX('v', 'm', 'h', 'd'),
  BOX_SMHD = MKBOX('s', 'm', 'h', 'd'),
  BOX_MDAT = MKBOX('m', 'd', 'a', 't'),
  BOX_FREE = MKBOX('f', 'r', 'e', 'e')
};

union box {
  struct box_top * top;
  struct box_ftyp * ftyp;
  struct box_moov * moov;
  struct box_trak * trak;
  struct box_edts * edts;
  struct box_mdia * mdia;
  struct box_minf * minf;
  struct box_dinf * dinf;
  struct box_dref * dref;
  struct box_dref_entry * dref_entry;
  struct box_stsd * stsd;
  struct box_vide * vide;
  struct box_soun * soun;
  struct box_vmhd * vmhd;
  struct box_smhd * smhd;
};

typedef union box box_t;

struct box_ftyp {
  unsigned int m_brand; /* major brand */
  unsigned int m_version; /* minor version */
  unsigned int * c_brands; /* compatible brands */
  unsigned int c_brands_len;
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

struct initial_object_descr {
  unsigned short object_descr_id;
  unsigned char url_flag;
  unsigned char include_inline_profile_level_flag;
  unsigned char od_profile_level_idc;
  unsigned char scene_profile_level_idc;
  unsigned char audio_profile_level_idc;
  unsigned char visual_profile_level_idc;
  unsigned char graphics_profile_levvel_idc;
};

struct box_iods {
  struct initial_object_descr iod;
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

struct vui {
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
  unsigned char max_bytes_per_pic_denom;
  unsigned char max_bits_per_mb_denom;
  unsigned char log2_max_mv_length_horizontal;
  unsigned char log2_max_mv_length_vertical;
  unsigned int num_reorder_frames;
  unsigned int max_dec_frame_buffering;
};

struct sps {
  unsigned char nal_ref_idc;
  unsigned char nal_unit_type;
  unsigned char profile_idc;
  unsigned char c_set0_flag;
  unsigned char c_set1_flag;
  unsigned char c_set2_flag;
  unsigned char c_set3_flag;
  unsigned char c_set4_flag;
  unsigned char c_set5_flag;
  unsigned char level_idc;
  unsigned char seq_para_set_id;
  unsigned char log2_max_frame_num_minus4;
  unsigned char pic_order_cnt_type;
  unsigned char log2_max_pic_order_cnt_lsb_minus4;
  unsigned int max_num_ref_frames;
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
  struct vui vui;
};

struct pps {
  unsigned char nal_ref_idc;
  unsigned char nal_unit_type;
  unsigned char pic_para_set_id;
  unsigned char seq_para_set_id;
  unsigned char entropy_coding_mode_flag;
  unsigned char pic_order_present_flag;
  unsigned char num_slice_groups_minus1;
  unsigned char num_ref_idx_10_active_minus1;
  unsigned char num_ref_idx_11_active_minus1;
  unsigned char weighted_pred_flag;
  unsigned char weighted_bipred_idc;
  unsigned char pic_init_qp_minus26;
  unsigned char pic_init_qs_minus26;
  unsigned char chroma_qp_index_offset;
  unsigned char deblocking_filter_control_present_flag;
  unsigned char constrained_intra_pred_flag;
  unsigned char redundant_pic_cnt_present_flag;
};

union nalu {
  struct sps * sps;
  struct pps * pps;
};

struct box_avcc {
  unsigned char conf_version;
  unsigned char profile_idc;
  unsigned char c_set0_flag;
  unsigned char c_set1_flag;
  unsigned char c_set2_flag;
  unsigned char c_set3_flag;
  unsigned char c_set4_flag;
  unsigned char c_set5_flag;
  unsigned char level_idc;
  unsigned char len_size_minus_one;
  unsigned char num_of_sps;
  unsigned char num_of_pps;
  union nalu sps;
  union nalu pps;
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
  struct box_avcc avcc;
};

struct audio_specific_config {
  unsigned int tag_len;
  unsigned char audio_object_type;
  unsigned char sampling_frequency_index;
  unsigned int sampling_frequency;
  unsigned char channel_configuration;
};

struct decoder_config_descr {
  unsigned char object_type_idc;
  unsigned char stream_type;
  unsigned char up_stream;
  unsigned int buffer_size_db;
  unsigned int max_bitrate;
  unsigned int avg_bitrate;
  struct audio_specific_config audio;
};

struct sl_config_descr {
  unsigned char predefined;
};

struct es_descr {
  unsigned short es_id;
  unsigned char es_flags;
  struct decoder_config_descr dec_conf;
  struct sl_config_descr sl_conf;
};

struct box_esds {
  struct es_descr es;
};

struct box_soun {
  unsigned short dref_index;
  unsigned short channelcount;
  unsigned short samplesize;
  unsigned int samplerate;
  struct box_esds esds;
};

struct box_stsd {
  unsigned int entry_count;
  box_t entry;
};

struct stts_entry {
  unsigned int sample_count;
  unsigned int sample_delta;
};

struct box_stts {
  unsigned int entry_count;
  struct stts_entry * entry;
};

struct ctts_entry {
  unsigned int sample_count;
  unsigned int sample_offset;
};

struct box_ctts {
  unsigned int entry_count;
  struct ctts_entry * entry;
};

struct stsc_entry {
  unsigned int first_chunk;
  unsigned int samples_per_chunk;
  unsigned int sample_desc_index;
};

struct box_stsc {
  unsigned int entry_count;
  struct stsc_entry * entry;
};

struct stco_entry {
  unsigned int chunk_offset;
  unsigned int samples_per_chunk; /* created from stsc_entry */
};

struct box_stco {
  unsigned int entry_count;
  long pos;
  struct stco_entry * entry;
};

struct stsz_entry {
  unsigned int pos;
  unsigned int entry_size;
};

struct box_stsz {
  unsigned int sample_size;
  unsigned int sample_count;
  struct stsz_entry * entry;
};

struct stss_entry {
  unsigned int sample_number;
};

struct box_stss {
  unsigned int entry_count;
  struct stss_entry * entry;
};

struct box_stbl {
  struct box_stsd stsd;
  struct box_stts stts;
  struct box_ctts ctts;
  struct box_stsc stsc;
  struct box_stco stco;
  struct box_stsz stsz;
  struct box_stss stss;
};

struct box_vmhd {
  unsigned short graphicsmode;
  unsigned short opcolor[3];
};

struct box_smhd {
  short balance;
};

struct box_minf {
  struct box_dinf dinf;
  struct box_stbl stbl;
  box_t hd;
};

struct elst_entry {
  unsigned int segment_duration;
  int media_time;
  short media_rate_integer;
  short media_rate_fraction;
};

struct box_elst {
  unsigned int entry_count;
  struct elst_entry * entry;
};

struct box_edts {
  struct box_elst elst;
};

struct box_mdia {
  struct box_mdhd mdhd;
  struct box_hdlr hdlr;
  struct box_minf minf;
};

struct box_trak {
  struct box_tkhd tkhd;
  struct box_edts edts;
  struct box_mdia mdia;
};

struct box_moov {
  struct box_mvhd mvhd;
  struct box_iods * iods;
  struct box_trak * trak;
  unsigned int trak_len;
};

struct box_mdat {
  FILE * file;
};

struct box_top {
  struct box_ftyp ftyp;
  struct box_moov moov;
  struct box_mdat mdat;
};

struct box_info {
  unsigned int dump;
  unsigned int depth;
  unsigned int size;
  unsigned int type;
  long pos;
};

typedef int (* box_func_t)(FILE * file, struct box_info * info, box_t p_box);

struct box_func {
  unsigned int name;
  unsigned int count;
  unsigned int qty; /* quantity */
  box_func_t func;
};

static const char *
box_to_str(unsigned int x, char * s) {
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
    "entry_count do not match",
    "Empty bits",
    "No avcc",
    "Unknown codec",
    "Illegal string length",
    "Unable to write bit",
    "Unable to write bits",
    "Illegal quantity of box",
    "No sound track"
  };
  if (i < 0 || i >= ERR_LEN)
    return NULL;
  return errors[i];
}

static void
print_spaces(struct box_info * info) {
  unsigned int depth;
  unsigned int i;

  depth = info->depth;

  for (i = 0; i < depth; i++)
    printf("  ");
}

static void
print_name(const char * name, struct box_info * info) {
  enum { MAX_LEN = 25 };
  size_t len;
  size_t i;

  print_spaces(info);

  len = strlen(name);

  if (len)
    printf("%s: ", name); /* 1st row */
  else
    printf("  "); /* nth row, n >= 2 */

  if (len < MAX_LEN)
    for (i = len; i < MAX_LEN; i++)
      putchar(' ');
}

void
print_c24(const char * name, const char * c, struct box_info * info) {
  print_name(name, info); printf("%.3s\n", c);
}

#define PRINT_C24(name, info) print_c24(#name, (name), (info))

void
print_box(const char * name, unsigned int x, struct box_info * info) {
  char str[4];
  print_name(name, info); printf("%.4s\n", box_to_str(x, str));
}

void
print_str(const char * name, const char * str, struct box_info * info) {
  print_name(name, info); printf("%s\n", str);
}

#define PRINT_STR(name, info) print_str(#name, (name), (info))

void
print_u(const char * name, unsigned int x, struct box_info * info) {
  print_name(name, info); printf("%u\n", x);
}

#define PRINT_U(name, info) print_u(#name, (name), (info))

void
print_s(const char * name, int s, struct box_info * info) {
  print_name(name, info); printf("%d\n", s);
}

#define PRINT_S(name, info) print_s(#name, (name), (info))

void
print_s_8(const char * name, int s, struct box_info * info) {
  print_name(name, info); printf("%d.%x\n", s >> 8, s & 0xff);
}

#define PRINT_S_8(name, info) print_s_8(#name, (name), (info))

void
print_s_16(const char * name, int s, struct box_info * info) {
  print_name(name, info); printf("%d.%x\n", s >> 16, s & 0xffff);
}

#define PRINT_S_16(name, info) print_s_16(#name, (name), (info))

void
print_u_16(const char * name, unsigned int u, struct box_info * info) {
  print_name(name, info); printf("%u.%x\n", u >> 16, u & 0xffff);
}

#define PRINT_U_16(name, info) print_u_16(#name, (name), (info))

void
print_mat(const char * name, int * matrix, struct box_info * info) {
  unsigned int i;

  for (i = 0; i < 3; i++) {
    print_name(i == 0 ? name : "", info);
    printf("%d.%x %d.%x %d.%x\n",
           matrix[i] >> 16, matrix[i] & 0xffff,
           matrix[i+3] >> 16, matrix[i+3] & 0xffff,
           matrix[i+6] >> 30, matrix[i+6] & 0x3fffffff);
  }
}

#define PRINT_MAT(name, info) print_mat(#name, (name), (info))

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
set_pos(long pos, FILE * file) {
  if (fseek(file, pos, SEEK_SET) == -1)
    return ERR_IO;
  return 0;
}

static int
read_ary(void * ptr, size_t size, size_t len, FILE * file) {
  if (fread(ptr, size, len, file) != len)
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

  * version = (unsigned char) (b32 >> 24);
  * flags = b32 & 0x00ffffff;
  return 0;
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
         struct box_func * funcs) {
  struct box_info child;
  char str[4];
  size_t i;
  int ret;

  child.dump = info->dump;
  child.depth = info->depth + 1;

  for (;;) {
    if ((ret = get_pos(&child.pos, file)) != 0)
      return ret;

    if ((unsigned int) (child.pos - info->pos) > info->size)
      return ERR_BOX_SIZE;
    if ((unsigned int) (child.pos - info->pos) == info->size)
      break;

    if ((ret = read_u32(&child.size, file)) != 0 ||
        (ret = read_u32(&child.type, file)) != 0)
      return ret;

    for (i = 0; funcs[i].name; i++)
      if (funcs[i].name == child.type)
        break;

    if (info->dump) {
      print_spaces(info);
      printf("[%.4s %u]\n", box_to_str(child.type, str), child.size);
    }

    if (funcs[i].name == 0)
      return ERR_UNK_BOX;

    if (funcs[i].qty & (BOX_QTY_0_OR_1 | BOX_QTY_1))
      if (funcs[i].count == 1)
        return ERR_BOX_QTY;

    if ((ret = funcs[i].func(file, &child, box)) != 0)
      return ret;

    funcs[i].count++;
  }

  for (i = 0; funcs[i].name; i++)
    if (funcs[i].qty & (BOX_QTY_1 | BOX_QTY_1_TO_N))
      if (funcs[i].count == 0)
        return ERR_BOX_QTY;

  return 0;
}

static int
read_iods(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned char tag;
  unsigned int tag_len;
  unsigned short object_descr_id;
  unsigned char url_flag;
  unsigned char include_inline_profile_level_flag;
  unsigned char od_profile_level_idc;
  unsigned char scene_profile_level_idc;
  unsigned char audio_profile_level_idc;
  unsigned char visual_profile_level_idc;
  unsigned char graphics_profile_levvel_idc;
  struct box_iods * iods;
  struct initial_object_descr * iod;
  int ret;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_tag(&tag, &tag_len, file)) != 0)
    return ret;

  if (tag != TAG_MP4_IOD)
    return ERR_UNK_TAG;

  if (info->dump) {
    print_spaces(info);
    printf("[InitialObjectDescr %u]\n", tag_len);
    info->depth++;
  }

  if ((ret = read_u16(&object_descr_id, file)) != 0)
    return ret;

  url_flag = (unsigned char) ((object_descr_id >> 5) & 0x1);
  include_inline_profile_level_flag =
      (unsigned char) ((object_descr_id >> 4) & 0x1);
  object_descr_id >>= 6;

  if (info->dump) {
    PRINT_U(object_descr_id, info);
    PRINT_U(url_flag, info);
    PRINT_U(include_inline_profile_level_flag, info);
  }

  if (url_flag == 0) {
    if ((ret = read_u8(&od_profile_level_idc, file)) != 0 ||
        (ret = read_u8(&scene_profile_level_idc, file)) != 0 ||
        (ret = read_u8(&audio_profile_level_idc, file)) != 0 ||
        (ret = read_u8(&visual_profile_level_idc, file)) != 0 ||
        (ret = read_u8(&graphics_profile_levvel_idc, file)) != 0)
      return ret;

    if (info->dump) {
      PRINT_U(od_profile_level_idc, info);
      PRINT_U(scene_profile_level_idc, info);
      PRINT_U(audio_profile_level_idc, info);
      PRINT_U(visual_profile_level_idc, info);
      PRINT_U(graphics_profile_levvel_idc, info);
    }
  }

  if (info->dump)
    info->depth--;

  if ((ret = mem_alloc(&iods, sizeof(* iods))) != 0)
    return ret;

  iod = &iods->iod;
  iod->object_descr_id = object_descr_id;
  iod->url_flag = url_flag;
  iod->include_inline_profile_level_flag = include_inline_profile_level_flag;
  if (url_flag == 0) {
    iod->od_profile_level_idc = od_profile_level_idc;
    iod->scene_profile_level_idc = scene_profile_level_idc;
    iod->audio_profile_level_idc = audio_profile_level_idc;
    iod->visual_profile_level_idc = visual_profile_level_idc;
    iod->graphics_profile_levvel_idc = graphics_profile_levvel_idc;
  }

  p_box.moov->iods = iods;
  return 0;
}

static int
read_udta(FILE * file, struct box_info * info, box_t p_box) {
  long pos;
  int ret;

  (void) p_box;

  if ((ret = get_pos(&pos, file)) != 0 ||
      (ret = skip(file, info->size - (unsigned int) (pos - info->pos))) != 0)
    return ret;
  return 0;
}

static int
read_ftyp(FILE * file, struct box_info * info, box_t p_box) {
  long pos;

  unsigned int m_brand; /* major brand */
  unsigned int m_version; /* minor version */
  unsigned int * c_brands; /* compatible brands */

  struct box_ftyp * ftyp;

  unsigned int i;
  unsigned int len;
  int ret;

  ret = 0;
  c_brands = NULL;

  if ((ret = read_u32(&m_brand, file)) != 0 ||
      (ret = read_u32(&m_version, file)) != 0 ||
      (ret = get_pos(&pos, file)) != 0)
    goto exit;

  if (info->dump) {
    print_box("major_brand", m_brand, info);
    print_u("minor_version", m_version, info);
  }

  len = (info->size - (unsigned int) (pos - info->pos)) / 4;

  if ((ret = mem_alloc(&c_brands, len * sizeof(c_brands[0]))) != 0)
    goto exit;

  for (i = 0; i < len; i++)
    if ((ret = read_u32(&c_brands[i], file)) != 0)
      goto free;

  if (info->dump)
    for (i = 0; i < len; i++)
      print_box("compatible brand", c_brands[i], info);

  ftyp = &p_box.top->ftyp;
  ftyp->m_brand = m_brand;
  ftyp->m_version = m_version;
  ftyp->c_brands = c_brands;
  ftyp->c_brands_len = len;
free:
  if (ret)
    mem_free(c_brands);
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

  if (info->dump) {
    print_u("creation time", c_time, info);
    print_u("modification time", m_time, info);
    PRINT_U(timescale, info);
    PRINT_U(duration, info);
    PRINT_S_16(rate, info);
    PRINT_S_8(volume, info);
    PRINT_MAT(matrix, info);
    PRINT_U(next_track_id, info);
  }

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

  if (info->dump) {
    PRINT_U(track_enabled, info);
    PRINT_U(track_in_movie, info);
    PRINT_U(track_in_preview, info);
    print_u("creation time", c_time, info);
    print_u("modification time", m_time, info);
    PRINT_U(track_id, info);
    PRINT_U(duration, info);
    PRINT_S(layer, info);
    PRINT_S(alternate_group, info);
    PRINT_S_8(volume, info);
    PRINT_MAT(matrix, info);
    PRINT_U_16(width, info);
    PRINT_U_16(height, info);
  }

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
read_elst(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int segment_duration;
  int media_time;
  short media_rate_integer;
  short media_rate_fraction;
  struct elst_entry * entry;
  struct box_elst * elst;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0 ||
      (ret = mem_alloc(&entry, entry_count * sizeof(* entry))) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&segment_duration, file)) != 0 ||
        (ret = read_s32(&media_time, file)) != 0 ||
        (ret = read_s16(&media_rate_integer, file)) != 0 ||
        (ret = read_s16(&media_rate_fraction, file)) != 0)
      goto free;

    entry[i].segment_duration = segment_duration;
    entry[i].media_time = media_time;
    entry[i].media_rate_integer = media_rate_integer;
    entry[i].media_rate_fraction = media_rate_fraction;

    if (info->dump) {
      PRINT_U(segment_duration, info);
      PRINT_S(media_time, info);
      PRINT_S(media_rate_integer, info);
      PRINT_S(media_rate_fraction, info);
    }
  }

  elst = &p_box.edts->elst;
  elst->entry_count = entry_count;
  elst->entry = entry;
free:
  if (ret)
    mem_free(entry);
exit:
  return ret;
}

static int
read_edts(FILE * file, struct box_info * info, box_t p_box) {
  struct box_func funcs[] = {
    {BOX_ELST, 0, BOX_QTY_0_OR_1, read_elst},
    {0, 0, 0, NULL}
  };
  box_t box;
  box.edts = &p_box.trak->edts;
  return read_box(file, info, box, funcs);
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

  if (info->dump) {
    print_u("creation time", c_time, info);
    print_u("modification time", m_time, info);
    PRINT_U(timescale, info);
    PRINT_U(duration, info);
    PRINT_C24(lang, info);
  }

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

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = skip(file, 4)) != 0 || /* pre defined */
      (ret = read_u32(&type, file)) != 0 ||
      (ret = skip(file, 4 * 3)) != 0 || /* reserved */
      (ret = read_str(&name, file)) != 0)
    return ret;

  if (info->dump) {
    print_box("handler_type", type, info);
    PRINT_STR(name, info);
  }

  hdlr = &p_box.mdia->hdlr;
  hdlr->type = type;
  hdlr->name = name;

  return 0;
}

static int
read_dref_entry(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned char self_contained;
  char * name;
  char * location;
  struct box_dref * dref;
  struct box_dref_entry * entry;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0)
    goto exit;

  self_contained = flags & 0x1;

  if (info->dump)
    PRINT_U(self_contained, info);

  dref = p_box.dref;
  if ((ret = mem_realloc(&dref->entry, (dref->entry_count + 1) *
                         sizeof(* dref->entry))) != 0)
    goto exit;

  entry = &dref->entry[dref->entry_count];

  name = NULL;
  location = NULL;

  if (info->type == BOX_URL) {

    if (self_contained == 0) {

      if ((ret = read_str(&location, file)) != 0)
        goto exit;

      if (info->dump)
        PRINT_STR(location, info);
    }
  } else if (info->type == BOX_URN) {

    if ((ret = read_str(&name, file)) != 0)
      goto exit;
    if ((ret = read_str(&location, file)) != 0)
      goto free;

    if (info->dump) {
      PRINT_STR(name, info);
      PRINT_STR(location, info);
    }
free:
    if (ret) {
      mem_free(name);
      goto exit;
    }
  } else {
    ret = ERR_UNK_BOX;
    goto exit;
  }
  entry->type = info->type;
  entry->self_contained = self_contained;
  entry->name = name;
  entry->location = location;

  dref->entry_count++;
exit:
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
  struct box_func funcs[] = {
    {BOX_URL, 0, BOX_QTY_0_TO_N, read_dref_entry},
    {BOX_URN, 0, BOX_QTY_0_TO_N, read_dref_entry},
    {0, 0, 0, NULL}
  };
  struct box_dref * dref;
  box_t box;
  int ret;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    return ret;

  if (info->dump)
    PRINT_U(entry_count, info);

  box.dref = dref = &p_box.dinf->dref;
  if ((ret = read_box(file, info, box, funcs)) != 0)
    return ret;

  if (dref->entry_count != entry_count)
    return ERR_ENTRY_COUNT;

  return 0;
}

static int
read_dinf(FILE * file, struct box_info * info, box_t p_box) {
  struct box_func funcs[] = {
    {BOX_DREF, 0, BOX_QTY_1, read_dref},
    {0, 0, 0, NULL}
  };
  box_t box;
  box.dinf = &p_box.mdia->minf.dinf;
  return read_box(file, info, box, funcs);
}

struct bits {
  unsigned char * bytes;
  unsigned int size;
  unsigned int i; /* index of unread bytes */
  unsigned int pos;
  unsigned int buf;
};

static int
read_bits_init(struct bits * bits, unsigned char * bytes, unsigned int size) {
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
  unsigned int c; /* a copy of unsigned byte */

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
  c = u << pos;
  while ((c & 0x80) == 0) { /* read & count zeros (bit) */
    c <<= 1;
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
  if (w < 1 || w > 32)
    return ERR_READ_BITS;

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

  PRINT_U(cpb_cnt_minus1, info);

exit:
  return ret;
}
#endif

static int
read_vui_para(struct vui * vui, struct bits * bits, struct box_info * info) {
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
  unsigned char max_bytes_per_pic_denom;
  unsigned char max_bits_per_mb_denom;
  unsigned char log2_max_mv_length_horizontal;
  unsigned char log2_max_mv_length_vertical;
  unsigned int num_reorder_frames;
  unsigned int max_dec_frame_buffering;

  int ret;

  ret = 0;

  if ((ret = read_bit(&aspect_ratio_info_present_flag, bits)) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(aspect_ratio_info_present_flag, info);

  if (aspect_ratio_info_present_flag) {
    if ((ret = read_bits_8(&aspect_ratio_idc, 8, bits)) != 0)
      goto exit;

    if (info->dump) {
      info->depth++;
      PRINT_U(aspect_ratio_idc, info);
      info->depth--;
    }

    if (aspect_ratio_idc == 0xff) {
      ret = ERR_UNK_ASPECT_RATIO_IDC;
      goto exit;
    }
  }

  if ((ret = read_bit(&overscan_info_present_flag, bits)) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(overscan_info_present_flag, info);

  if (overscan_info_present_flag) {
    ret = ERR_UNK_OVERSCAN_INFO_PRESENT_FLAG;
    goto exit;
  }

  if ((ret = read_bit(&video_signal_type_present_flag, bits)) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(video_signal_type_present_flag, info);

  if (video_signal_type_present_flag) {
    if ((ret = read_bits_8(&video_format, 3, bits)) != 0 ||
        (ret = read_bit(&video_full_range_flag, bits)) != 0 ||
        (ret = read_bit(&colour_description_present_flag, bits)) != 0)
      goto exit;

    if (info->dump) {
      info->depth++;
      PRINT_U(video_format, info);
      PRINT_U(video_full_range_flag, info);
      PRINT_U(colour_description_present_flag, info);
    }

    if (colour_description_present_flag) {
      if ((ret = read_bits_8(&colour_primaries, 8, bits)) != 0 ||
          (ret = read_bits_8(&transfer_characteristics, 8, bits)) != 0 ||
          (ret = read_bits_8(&matrix_coefficients, 8, bits)) != 0)
        goto exit;

      if (info->dump) {
        info->depth++;
        PRINT_U(colour_primaries, info);
        PRINT_U(transfer_characteristics, info);
        PRINT_U(matrix_coefficients, info);
        info->depth--;
      }
    }
    if (info->dump)
      info->depth--;
  }

  if ((ret = read_bit(&chroma_loc_info_present_flag, bits)) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(chroma_loc_info_present_flag, info);

  if (chroma_loc_info_present_flag) {
    ret = ERR_UNK_CHROMA_LOC_INFO_PRESENT_FLAG;
    goto exit;
  }

  if ((ret = read_bit(&timing_info_present_flag, bits)) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(timing_info_present_flag, info);

  if (timing_info_present_flag) {
    if ((ret = read_bits(&num_units_in_tick, 32, bits)) != 0 ||
        (ret = read_bits(&time_scale, 32, bits)) != 0 ||
        (ret = read_bit(&fixed_frame_rate_flag, bits)) != 0)
      goto exit;

    if (info->dump) {
      info->depth++;
      PRINT_U(num_units_in_tick, info);
      PRINT_U(time_scale, info);
      PRINT_U(fixed_frame_rate_flag, info);
      info->depth--;
    }
  }

  if ((ret = read_bit(&nal_hrd_para_present_flag, bits)) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(nal_hrd_para_present_flag, info);

  if (nal_hrd_para_present_flag) {
    ret = ERR_UNK_NAL_HRD_PARA_PRESENT_FLAG;
    goto exit;
  }

  if ((ret = read_bit(&vcl_hrd_para_present_flag, bits)) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(vcl_hrd_para_present_flag, info);

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

  if (info->dump) {
    PRINT_U(pic_struct_present_flag, info);
    PRINT_U(bitstream_restriction_flag, info);
  }

  if (bitstream_restriction_flag) {
    if ((ret = read_bit(&motion_vectors_over_pic_boundaries_flag, bits)) != 0 ||
        (ret = read_code_8(&max_bytes_per_pic_denom, bits)) != 0 ||
        (ret = read_code_8(&max_bits_per_mb_denom, bits)) != 0 ||
        (ret = read_code_8(&log2_max_mv_length_horizontal, bits)) != 0 ||
        (ret = read_code_8(&log2_max_mv_length_vertical, bits)) != 0 ||
        (ret = read_code(&num_reorder_frames, bits)) != 0 ||
        (ret = read_code(&max_dec_frame_buffering, bits)) != 0)
      goto exit;

    if (info->dump) {
      info->depth++;
      PRINT_U(motion_vectors_over_pic_boundaries_flag, info);
      PRINT_U(max_bytes_per_pic_denom, info);
      PRINT_U(max_bits_per_mb_denom, info);
      PRINT_U(log2_max_mv_length_horizontal, info);
      PRINT_U(log2_max_mv_length_vertical, info);
      PRINT_U(num_reorder_frames, info);
      PRINT_U(max_dec_frame_buffering, info);
      info->depth--;
    }
  }

  vui->aspect_ratio_info_present_flag = aspect_ratio_info_present_flag;
  if (aspect_ratio_info_present_flag)
    vui->aspect_ratio_idc = aspect_ratio_idc;

  vui->overscan_info_present_flag = overscan_info_present_flag;

  vui->video_signal_type_present_flag = video_signal_type_present_flag;
  if (video_signal_type_present_flag) {
    vui->video_format = video_format;
    vui->video_full_range_flag = video_full_range_flag;

    vui->colour_description_present_flag = colour_description_present_flag;
    if (colour_description_present_flag) {
      vui->colour_primaries = colour_primaries;
      vui->transfer_characteristics = transfer_characteristics;
      vui->matrix_coefficients = matrix_coefficients;
    }
  }

  vui->chroma_loc_info_present_flag = chroma_loc_info_present_flag;

  vui->timing_info_present_flag = timing_info_present_flag;
  if (timing_info_present_flag) {
    vui->num_units_in_tick = num_units_in_tick;
    vui->time_scale = time_scale;
    vui->fixed_frame_rate_flag = fixed_frame_rate_flag;
  }

  vui->nal_hrd_para_present_flag = nal_hrd_para_present_flag;
  vui->vcl_hrd_para_present_flag = vcl_hrd_para_present_flag;
  vui->pic_struct_present_flag = pic_struct_present_flag;

  vui->bitstream_restriction_flag = bitstream_restriction_flag;
  if (bitstream_restriction_flag) {
    vui->motion_vectors_over_pic_boundaries_flag =
        motion_vectors_over_pic_boundaries_flag;
    vui->max_bytes_per_pic_denom = max_bytes_per_pic_denom;
    vui->max_bits_per_mb_denom = max_bits_per_mb_denom;
    vui->log2_max_mv_length_horizontal = log2_max_mv_length_horizontal;
    vui->log2_max_mv_length_vertical = log2_max_mv_length_vertical;
    vui->num_reorder_frames = num_reorder_frames;
    vui->max_dec_frame_buffering = max_dec_frame_buffering;
  }
exit:
  return ret;
}

static int
read_nalu(union nalu ret_nalu, unsigned int size, FILE * file,
          struct box_info * info) {
  unsigned char nal_ref_idc;
  unsigned char nal_unit_type;
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

  nal_ref_idc = (unsigned char) ((nalu[0] >> 5) & 0x3);
  nal_unit_type = (unsigned char) (nalu[0] & 0x1f);

  if (info->dump) {
    PRINT_U(nal_ref_idc, info);
    PRINT_U(nal_unit_type, info);
  }

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

  if (nal_unit_type == NAL_SPS) {
    unsigned char profile_idc; /* AVC profile indication */
    unsigned char profile_comp; /* profile compatibility */
    unsigned char c_set0_flag; /* constraint set 0 flag */
    unsigned char c_set1_flag; /* constraint set 1 flag */
    unsigned char c_set2_flag; /* constraint set 2 flag */
    unsigned char c_set3_flag; /* constraint set 3 flag */
    unsigned char c_set4_flag; /* constraint set 4 flag */
    unsigned char c_set5_flag; /* constraint set 5 flag */
    unsigned char level_idc; /* AVC level indication */
    unsigned char seq_para_set_id; /* seq parameter set id */
    unsigned char log2_max_frame_num_minus4; /* MaxFrameNum used in frame_num */
    unsigned char pic_order_cnt_type; /* to decode picture order count */
    unsigned char log2_max_pic_order_cnt_lsb_minus4;
    unsigned int max_num_ref_frames;
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
    struct sps * sps;

    if ((ret = read_bits_init(&bits, rbsp, rbsp_size)) != 0 ||
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
    c_set3_flag = (unsigned char) ((profile_comp >> 4) & 0x1);
    c_set4_flag = (unsigned char) ((profile_comp >> 3) & 0x1);
    c_set5_flag = (unsigned char) ((profile_comp >> 2) & 0x1);

    if (info->dump) {
      PRINT_U(profile_idc, info);
      print_u("constraint_set0_flag", c_set0_flag, info);
      print_u("constraint_set1_flag", c_set1_flag, info);
      print_u("constraint_set2_flag", c_set2_flag, info);
      print_u("constraint_set3_flag", c_set3_flag, info);
      print_u("constraint_set4_flag", c_set4_flag, info);
      print_u("constraint_set5_flag", c_set5_flag, info);
      PRINT_U(level_idc, info);
      PRINT_U(seq_para_set_id, info);
      PRINT_U(log2_max_frame_num_minus4, info);
      PRINT_U(pic_order_cnt_type, info);
    }

    if (pic_order_cnt_type == 0) {
      if ((ret = read_code_8(&log2_max_pic_order_cnt_lsb_minus4, &bits)) != 0)
        goto free;

      if (info->dump)
        PRINT_U(log2_max_pic_order_cnt_lsb_minus4, info);
    } else if (pic_order_cnt_type == 2) {
    } else {
      ret = ERR_UNK_PIC_ORDER_CNT_TYPE;
      goto free;
    }

    if ((ret = read_code(&max_num_ref_frames, &bits)) != 0 ||
        (ret = read_bit(&gaps_in_frame_num_value_allowed_flag, &bits)) != 0 ||
        (ret = read_code(&pic_width_in_mbs_minus_1, &bits)) != 0 ||
        (ret = read_code(&pic_height_in_mbs_minus_1, &bits)) != 0 ||
        (ret = read_bit(&frame_mbs_only_flag, &bits)) != 0)
      goto free;

    if (info->dump) {
      PRINT_U(max_num_ref_frames, info);
      PRINT_U(gaps_in_frame_num_value_allowed_flag, info);
      PRINT_U(pic_width_in_mbs_minus_1, info);
      PRINT_U(pic_height_in_mbs_minus_1, info);
      PRINT_U(frame_mbs_only_flag, info);
    }

    if (!frame_mbs_only_flag) {
      if ((ret = read_bit(&mb_adaptive_frame_field_flag, &bits)) != 0)
        goto free;

      if (info->dump)
        PRINT_U(mb_adaptive_frame_field_flag, info);
    }

    if ((ret = read_bit(&direct_8x8_inference_flag, &bits)) != 0 ||
        (ret = read_bit(&frame_cropping_flag, &bits)) != 0)
      goto free;

    if (info->dump) {
      PRINT_U(direct_8x8_inference_flag, info);
      PRINT_U(frame_cropping_flag, info);
    }

    if (frame_cropping_flag) {
      if ((ret = read_code(&frame_crop_left_offset, &bits)) != 0 ||
          (ret = read_code(&frame_crop_right_offset, &bits)) != 0 ||
          (ret = read_code(&frame_crop_top_offset, &bits)) != 0 ||
          (ret = read_code(&frame_crop_bottom_offset, &bits)) != 0)
        goto free;

      if (info->dump) {
        PRINT_U(frame_crop_left_offset, info);
        PRINT_U(frame_crop_right_offset, info);
        PRINT_U(frame_crop_top_offset, info);
        PRINT_U(frame_crop_bottom_offset, info);
      }
    }

    sps = ret_nalu.sps;

    if ((ret = read_bit(&vui_para_present_flag, &bits)) != 0)
      goto free;

    if (info->dump)
      PRINT_U(vui_para_present_flag, info);

    if (vui_para_present_flag) {

      info->depth++;

      if ((ret = read_vui_para(&sps->vui, &bits, info)) != 0)
        goto free;

      info->depth--;
    }
    sps->nal_ref_idc = nal_ref_idc;
    sps->nal_unit_type = nal_unit_type;
    sps->profile_idc = profile_idc;
    sps->c_set0_flag = c_set0_flag;
    sps->c_set1_flag = c_set1_flag;
    sps->c_set2_flag = c_set2_flag;
    sps->c_set3_flag = c_set3_flag;
    sps->c_set4_flag = c_set4_flag;
    sps->c_set5_flag = c_set5_flag;
    sps->level_idc = level_idc;
    sps->seq_para_set_id = seq_para_set_id;
    sps->log2_max_frame_num_minus4 = log2_max_frame_num_minus4;
    sps->pic_order_cnt_type = pic_order_cnt_type;

    if (pic_order_cnt_type == 0)
      sps->log2_max_pic_order_cnt_lsb_minus4 =
          log2_max_pic_order_cnt_lsb_minus4;

    sps->max_num_ref_frames = max_num_ref_frames;
    sps->gaps_in_frame_num_value_allowed_flag =
        gaps_in_frame_num_value_allowed_flag;
    sps->pic_width_in_mbs_minus_1 = pic_width_in_mbs_minus_1;
    sps->pic_height_in_mbs_minus_1 = pic_height_in_mbs_minus_1;
    sps->frame_mbs_only_flag = frame_mbs_only_flag;

    if (!frame_mbs_only_flag)
      sps->mb_adaptive_frame_field_flag = mb_adaptive_frame_field_flag;

    sps->direct_8x8_inference_flag = direct_8x8_inference_flag;
    sps->frame_cropping_flag = frame_cropping_flag;

    if (frame_cropping_flag) {
      sps->frame_crop_left_offset = frame_crop_left_offset;
      sps->frame_crop_right_offset = frame_crop_right_offset;
      sps->frame_crop_top_offset = frame_crop_top_offset;
      sps->frame_crop_bottom_offset = frame_crop_bottom_offset;
    }
    sps->vui_para_present_flag = vui_para_present_flag;
  } else if (nal_unit_type == NAL_PPS) {
    unsigned char pic_para_set_id;
    unsigned char seq_para_set_id;
    unsigned char entropy_coding_mode_flag;
    unsigned char pic_order_present_flag;
    unsigned char num_slice_groups_minus1;
    unsigned char num_ref_idx_10_active_minus1;
    unsigned char num_ref_idx_11_active_minus1;
    unsigned char weighted_pred_flag;
    unsigned char weighted_bipred_idc;
    unsigned char pic_init_qp_minus26;
    unsigned char pic_init_qs_minus26;
    unsigned char chroma_qp_index_offset;
    unsigned char deblocking_filter_control_present_flag;
    unsigned char constrained_intra_pred_flag;
    unsigned char redundant_pic_cnt_present_flag;
    struct pps * pps;

    if ((ret = read_bits_init(&bits, rbsp, rbsp_size)) != 0 ||
        (ret = read_code_8(&pic_para_set_id, &bits)) != 0 ||
        (ret = read_code_8(&seq_para_set_id, &bits)) != 0 ||
        (ret = read_bit(&entropy_coding_mode_flag, &bits)) != 0 ||
        (ret = read_bit(&pic_order_present_flag, &bits)) != 0 ||
        (ret = read_code_8(&num_slice_groups_minus1, &bits)) != 0)
      goto free;

    if (info->dump) {
      PRINT_U(pic_para_set_id, info);
      PRINT_U(seq_para_set_id, info);
      PRINT_U(entropy_coding_mode_flag, info);
      PRINT_U(pic_order_present_flag, info);
      PRINT_U(num_slice_groups_minus1, info);
    }

    if (num_slice_groups_minus1 > 0) {
      ret = ERR_UNK_NUM_SLICE_GROUPS_MINUS1;
      goto free;
    }

    if ((ret = read_code_8(&num_ref_idx_10_active_minus1, &bits)) != 0 ||
        (ret = read_code_8(&num_ref_idx_11_active_minus1, &bits)) != 0 ||
        (ret = read_bit(&weighted_pred_flag, &bits)) != 0 ||
        (ret = read_bits_8(&weighted_bipred_idc, 2, &bits)) != 0 ||
        (ret = read_code_8(&pic_init_qp_minus26, &bits)) != 0 ||
        (ret = read_code_8(&pic_init_qs_minus26, &bits)) != 0 ||
        (ret = read_code_8(&chroma_qp_index_offset, &bits)) != 0 ||
        (ret = read_bit(&deblocking_filter_control_present_flag, &bits)) != 0 ||
        (ret = read_bit(&constrained_intra_pred_flag, &bits)) != 0 ||
        (ret = read_bit(&redundant_pic_cnt_present_flag, &bits)) != 0)
      goto free;

    if (info->dump) {
      PRINT_U(num_ref_idx_10_active_minus1, info);
      PRINT_U(num_ref_idx_11_active_minus1, info);
      PRINT_U(weighted_pred_flag, info);
      PRINT_U(weighted_bipred_idc, info);
      PRINT_U(pic_init_qp_minus26, info);
      PRINT_U(pic_init_qs_minus26, info);
      PRINT_U(chroma_qp_index_offset, info);
      PRINT_U(deblocking_filter_control_present_flag, info);
      PRINT_U(constrained_intra_pred_flag, info);
      PRINT_U(redundant_pic_cnt_present_flag, info);
    }

    pps = ret_nalu.pps;
    pps->nal_ref_idc = nal_ref_idc;
    pps->nal_unit_type = nal_unit_type;
    pps->pic_para_set_id = pic_para_set_id;
    pps->seq_para_set_id = seq_para_set_id;
    pps->entropy_coding_mode_flag = entropy_coding_mode_flag;
    pps->pic_order_present_flag = pic_order_present_flag;
    pps->num_slice_groups_minus1 = num_slice_groups_minus1;
    pps->num_ref_idx_10_active_minus1 = num_ref_idx_10_active_minus1;
    pps->num_ref_idx_11_active_minus1 = num_ref_idx_11_active_minus1;
    pps->weighted_pred_flag = weighted_pred_flag;
    pps->weighted_bipred_idc = weighted_bipred_idc;
    pps->pic_init_qp_minus26 = pic_init_qp_minus26;
    pps->pic_init_qs_minus26 = pic_init_qs_minus26;
    pps->chroma_qp_index_offset = chroma_qp_index_offset;
    pps->deblocking_filter_control_present_flag =
        deblocking_filter_control_present_flag;
    pps->constrained_intra_pred_flag = constrained_intra_pred_flag;
    pps->redundant_pic_cnt_present_flag = redundant_pic_cnt_present_flag;
  } else {
    ret = ERR_UNK_NAL_UNIT_TYPE;
    goto free;
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
  unsigned char c_set3_flag; /* constraint set 3 flag */
  unsigned char c_set4_flag; /* constraint set 4 flag */
  unsigned char c_set5_flag; /* constraint set 5 flag */
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

  struct box_avcc * avcc;
  struct sps * sps;
  struct pps * pps;
  union nalu nalu;
  unsigned int i;
  int ret;

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
  c_set3_flag = (unsigned char) ((profile_comp >> 4) & 0x1);
  c_set4_flag = (unsigned char) ((profile_comp >> 3) & 0x1);
  c_set5_flag = (unsigned char) ((profile_comp >> 2) & 0x1);
  len_size_minus_one = (unsigned char) (len_size_minus_one & 0x3);
  num_of_sps = (unsigned char) (num_of_sps & 0x1f);

  if (info->dump) {
    PRINT_U(conf_version, info);
    PRINT_U(profile_idc, info);
    print_u("constraint_set0_flag", c_set0_flag, info);
    print_u("constraint_set1_flag", c_set1_flag, info);
    print_u("constraint_set2_flag", c_set2_flag, info);
    print_u("constraint_set3_flag", c_set3_flag, info);
    print_u("constraint_set4_flag", c_set4_flag, info);
    print_u("constraint_set5_flag", c_set5_flag, info);
    PRINT_U(level_idc, info);
    PRINT_U(len_size_minus_one, info);
    PRINT_U(num_of_sps, info);
  }

  sps = NULL;
  pps = NULL;

  /* sps */

  if ((ret = mem_alloc(&sps, num_of_sps * sizeof(* sps))) != 0)
    goto free;

  for (i = 0; i < num_of_sps; i++) {
    if ((ret = read_u16(&sps_len, file)) != 0)
      goto free;

    if (info->dump)
      PRINT_U(sps_len, info);

    nalu.sps = &sps[i];

    info->depth++;

    if ((ret = read_nalu(nalu, sps_len, file, info)) != 0)
      goto free;

    info->depth--;
  }

  /* pps */

  if ((ret = read_u8(&num_of_pps, file)) != 0)
    goto free;

  if (info->dump)
    PRINT_U(num_of_pps, info);

  if ((ret = mem_alloc(&pps, num_of_pps * sizeof(* pps))) != 0)
    goto free;

  for (i = 0; i < num_of_pps; i++) {
    if ((ret = read_u16(&pps_len, file)) != 0)
      goto free;

    if (info->dump)
      PRINT_U(pps_len, info);

    nalu.pps = &pps[i];

    info->depth++;

    if ((ret = read_nalu(nalu, pps_len, file, info)) != 0)
      goto free;

    info->depth--;
  }

  if (profile_idc == 100 ||
      profile_idc == 110 ||
      profile_idc == 122 ||
      profile_idc == 144) {
    ret = ERR_UNK_PROFILE_IDC;
    goto free;
  }

  avcc = &p_box.vide->avcc;
  avcc->conf_version = conf_version;
  avcc->profile_idc = profile_idc;
  avcc->c_set0_flag = c_set0_flag;
  avcc->c_set1_flag = c_set1_flag;
  avcc->c_set2_flag = c_set2_flag;
  avcc->c_set3_flag = c_set3_flag;
  avcc->c_set4_flag = c_set4_flag;
  avcc->c_set5_flag = c_set5_flag;
  avcc->level_idc = level_idc;
  avcc->len_size_minus_one = len_size_minus_one;
  avcc->num_of_sps = num_of_sps;
  avcc->num_of_pps = num_of_pps;
  avcc->sps.sps = sps;
  avcc->pps.pps = pps;
free:
  if (ret) {
    mem_free(sps);
    mem_free(pps);
  }
exit:
  return ret;
}

static void
init_vide(struct box_vide * vide) {
  vide->avcc.sps.sps = NULL;
  vide->avcc.pps.pps = NULL;
}

static void
free_vide(struct box_vide * vide) {
  mem_free(vide->avcc.sps.sps);
  mem_free(vide->avcc.pps.pps);
}

static int
read_vide(FILE * file, struct box_info * info, box_t p_box) {
  unsigned short dref_index; /* data reference index */
  unsigned short width;
  unsigned short height;
  unsigned int h_rez;
  unsigned int v_rez;
  unsigned short frame_count;
  unsigned char len;
  char compressorname[32];
  unsigned short depth;
  struct box_func funcs[] = {
    {BOX_AVCC, 0, BOX_QTY_1, read_avcc},
    {0, 0, 0, NULL}
  };
  struct box_stsd * stsd;
  struct box_vide * vide;
  box_t box;
  int ret;

  ret = 0;

  if (p_box.mdia->hdlr.type != BOX_VIDE) {
    ret = ERR_UNK_HDLR_TYPE;
    goto exit;
  }

  stsd = &p_box.mdia->minf.stbl.stsd;
  if ((ret = mem_realloc(&stsd->entry.vide, (stsd->entry_count + 1) *
                         sizeof(* stsd->entry.vide))) != 0)
    goto exit;

  box.vide = vide = &stsd->entry.vide[stsd->entry_count];

  init_vide(vide);

  if ((ret = skip(file, 6)) != 0 || /* reserved */
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
    goto free;

  if (len >= 32) {
    ret = ERR_STR_LEN;
    goto free;
  }
  compressorname[len] = '\0';

  if (info->dump) {
    PRINT_U(dref_index, info);
    PRINT_U(width, info);
    PRINT_U(height, info);
    PRINT_U_16(h_rez, info);
    PRINT_U_16(v_rez, info);
    PRINT_U(frame_count, info);
    PRINT_STR(compressorname, info);
    PRINT_U(depth, info);
  }

  if ((ret = read_box(file, info, box, funcs)) != 0)
    goto free;

  vide->dref_index = dref_index;
  vide->width = width;
  vide->height = height;
  vide->h_rez = h_rez;
  vide->v_rez = v_rez;
  vide->frame_count = frame_count;
  memcpy(vide->compressorname, compressorname, sizeof(compressorname));
  vide->depth = depth;

  stsd->entry_count++;
free:
  if (ret)
    free_vide(vide);
exit:
  return ret;
}

static int
id_to_codec(unsigned char * ret_codec, unsigned char id) {
  struct pair {
    unsigned char id;
    unsigned char codec;
  };
  struct pair pairs[] = {
    {0x40, CODEC_AAC} /* Audio ISO/IEC 14496-3 */
  };
  unsigned int i;

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

  unsigned char object_type_idc;
  unsigned char codec;
  unsigned char stream_type;
  unsigned char up_stream;
  unsigned int buffer_size_db;
  unsigned int max_bitrate;
  unsigned int avg_bitrate;

  unsigned char * bytes;
  struct bits bits;

  unsigned int audio_tag_len;
  unsigned char audio_object_type;
  unsigned char sampling_frequency_index;
  unsigned int sampling_frequency;
  unsigned char channel_configuration;

  unsigned char predefined;

  struct box_esds * esds;
  struct es_descr * es;
  struct decoder_config_descr * dec;
  struct sl_config_descr * sl;
  struct audio_specific_config * audio;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_tag(&tag, &tag_len, file)) != 0)
    goto exit;

  if (tag != TAG_ES_DESCR) {
    ret = ERR_UNK_TAG;
    goto exit;
  }

  if (info->dump) {
    print_spaces(info);
    printf("[ES_Descr %u]\n", tag_len);
    info->depth++;
  }

  if ((ret = read_u16(&es_id, file)) != 0 ||
      (ret = read_u8(&es_flags, file)) != 0)
    goto exit;

  if (info->dump) {
    PRINT_U(es_id, info);
    PRINT_U(es_flags, info);
  }

  if (es_flags) {
    ret = ERR_UNK_ES_FLAGS;
    goto exit;
  }

  if ((ret = read_tag(&tag, &tag_len, file)) != 0)
    goto exit;

  if (tag != TAG_DECODER_CONFIG_DESCR) {
    ret = ERR_UNK_TAG;
    goto exit;
  }

  if (info->dump) {
    print_spaces(info);
    printf("[DecoderConfigDescr %u]\n", tag_len);
    info->depth++;
  }

  if ((ret = read_u8(&object_type_idc, file)) != 0 ||
      (ret = id_to_codec(&codec, object_type_idc)) != 0 ||
      (ret = read_u32(&buffer_size_db, file)) != 0 ||
      (ret = read_u32(&max_bitrate, file)) != 0 ||
      (ret = read_u32(&avg_bitrate, file)) != 0 ||
      (ret = read_tag(&tag, &tag_len, file)) != 0)
    goto exit;

  stream_type = (unsigned char) (buffer_size_db >> 26);
  up_stream = (buffer_size_db >> 25) & 0x1;
  buffer_size_db = buffer_size_db & 0xffffff; 

  if (info->dump) {
    PRINT_U(object_type_idc, info);
    PRINT_U(stream_type, info);
    PRINT_U(up_stream, info);
    PRINT_U(buffer_size_db, info);
    PRINT_U(max_bitrate, info);
    PRINT_U(avg_bitrate, info);
  }

  if (tag != TAG_DEC_SPECIFIC_INFO ||
      tag_len == 0) {
    ret = ERR_UNK_TAG;
    goto exit;
  }

  audio_tag_len = tag_len;

  if (info->dump) {
    print_spaces(info);
    printf("[AudioSpecificConfig %u]\n", tag_len);
    info->depth++;
  }

  if (codec == CODEC_AAC) {

    if ((ret = mem_alloc(&bytes, tag_len)) != 0)
      goto exit;

    if ((ret = read_ary(bytes, tag_len, 1, file)) != 0 ||
        (ret = read_bits_init(&bits, bytes, tag_len)) != 0 ||
        (ret = read_bits_8(&audio_object_type, 5, &bits)) != 0)
      goto free;

    if (audio_object_type == 0x1f) {
      if ((ret = read_bits_8(&audio_object_type, 6, &bits)) != 0)
        goto free;
      audio_object_type = (unsigned char) (audio_object_type + 0x20);
    }

    if (info->dump)
      PRINT_U(audio_object_type, info);

    if ((ret = read_bits_8(&sampling_frequency_index, 4, &bits)) != 0)
      goto free;

    if (info->dump)
      PRINT_U(sampling_frequency_index, info);

    if (sampling_frequency_index == 0xf) {
      if ((ret = read_bits(&sampling_frequency, 24, &bits)) != 0)
        goto free;

      if (info->dump)
        PRINT_U(sampling_frequency, info);
    } else {
      sampling_frequency = 0;
    }

    if ((ret = read_bits_8(&channel_configuration, 4, &bits)) != 0)
      goto free;

    if (info->dump)
      PRINT_U(channel_configuration, info);
free:
    mem_free(bytes);
    if (ret)
      goto exit;
  } else {
    ret = ERR_UNK_CODEC;
    goto exit;
  }

  if (info->dump)
    info->depth -= 2;

  if ((ret = read_tag(&tag, &tag_len, file)) != 0)
    goto exit;

  if (tag != TAG_SL_CONFIG_DESCR) {
    ret = ERR_UNK_TAG;
    goto exit;
  }

  if (info->dump) {
    print_spaces(info);
    printf("[SLConfigDescr %u]\n", tag_len);
    info->depth++;
  }

  if ((ret = read_u8(&predefined, file)) != 0)
    goto exit;

  if (info->dump) {
    PRINT_U(predefined, info);
    info->depth -= 2;
  }

  esds = &p_box.soun->esds;

  es = &esds->es;
  es->es_id = es_id;
  es->es_flags = es_flags;;

  dec = &es->dec_conf;
  dec->object_type_idc = object_type_idc;
  dec->stream_type = stream_type;
  dec->up_stream = up_stream;
  dec->buffer_size_db = buffer_size_db;
  dec->max_bitrate = max_bitrate;
  dec->avg_bitrate = avg_bitrate;

  audio = &dec->audio;
  audio->tag_len = audio_tag_len;
  audio->audio_object_type = audio_object_type;
  audio->sampling_frequency_index = sampling_frequency_index;
  audio->sampling_frequency = sampling_frequency;
  audio->channel_configuration = channel_configuration;

  sl = &es->sl_conf;
  sl->predefined = predefined;
exit:
  return ret;
}

static void
init_soun(struct box_soun * soun) {
  (void) soun;
}

static void
free_soun(struct box_soun * soun) {
  (void) soun;
}

static int
read_soun(FILE * file, struct box_info * info, box_t p_box) {
  unsigned short dref_index; /* data reference index */
  unsigned short channelcount;
  unsigned short samplesize;
  unsigned int samplerate;
  struct box_func funcs[] = {
    {BOX_ESDS, 0, BOX_QTY_1, read_esds},
    {0, 0, 0, NULL}
  };
  struct box_stsd * stsd;
  struct box_soun * soun;
  box_t box;
  int ret;

  ret = 0;

  if (p_box.mdia->hdlr.type != BOX_SOUN) {
    ret = ERR_UNK_HDLR_TYPE;
    goto exit;
  }

  stsd = &p_box.mdia->minf.stbl.stsd;
  if ((ret = mem_realloc(&stsd->entry.soun, (stsd->entry_count + 1) *
                         sizeof(* stsd->entry.soun))) != 0)
    goto exit;

  box.soun = soun = &stsd->entry.soun[stsd->entry_count];

  init_soun(soun);

  if ((ret = skip(file, 6)) != 0 || /* reserved */
      (ret = read_u16(&dref_index, file)) != 0 ||
      (ret = skip(file, 4 * 2)) != 0 || /* reserved */
      (ret = read_u16(&channelcount, file)) != 0 ||
      (ret = read_u16(&samplesize, file)) != 0 ||
      (ret = skip(file, 2 + 2)) != 0 || /* pre defined / reserved */
      (ret = read_u32(&samplerate, file)) != 0)
    goto free;

  if (info->dump) {
    PRINT_U(dref_index, info);
    PRINT_U(channelcount, info);
    PRINT_U(samplesize, info);
    PRINT_U_16(samplerate, info);
  }

  if ((ret = read_box(file, info, box, funcs)) != 0)
    goto free;

  soun->dref_index = dref_index;
  soun->channelcount = channelcount;
  soun->samplesize = samplesize;
  soun->samplerate = samplerate;

  stsd->entry_count++;
free:
  if (ret)
    free_soun(soun);
exit:
  return ret;
}

static int
read_stsd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  struct box_stsd * stsd;
  struct box_func funcs[] = {
    {BOX_AVC1, 0, BOX_QTY_0_TO_N, read_vide},
    {BOX_MP4A, 0, BOX_QTY_0_TO_N, read_soun},
    {0, 0, 0, NULL}
  };
  int ret;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0)
    return ret;

  if (info->dump)
    PRINT_U(entry_count, info);

  if ((ret = read_box(file, info, p_box, funcs)) != 0)
    return ret;

  stsd = &p_box.mdia->minf.stbl.stsd;
  if (stsd->entry_count != entry_count)
    return ERR_ENTRY_COUNT;

  return 0;
}

static int
read_stts(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int sample_count;
  unsigned int sample_delta;
  struct stts_entry * entry;
  struct box_stts * stts;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0 ||
      (ret = mem_alloc(&entry, entry_count * sizeof(* entry))) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&sample_count, file)) != 0 ||
        (ret = read_u32(&sample_delta, file)) != 0)
      goto free;

    if (info->dump) {
      print_spaces(info);
      printf("[%u]\n", i);

      info->depth++;

      PRINT_U(sample_count, info);
      PRINT_U(sample_delta, info);

      info->depth--;
    }

    entry[i].sample_count = sample_count;
    entry[i].sample_delta = sample_delta;
  }

  stts = &p_box.mdia->minf.stbl.stts;
  stts->entry_count = entry_count;
  stts->entry = entry;
free:
  if (ret)
    mem_free(entry);
exit:
  return ret;
}

static int
read_ctts(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int sample_count;
  unsigned int sample_offset;
  struct ctts_entry * entry;
  struct box_ctts * ctts;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0 ||
      (ret = mem_alloc(&entry, entry_count * sizeof(* entry))) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&sample_count, file)) != 0 ||
        (ret = read_u32(&sample_offset, file)) != 0)
      goto free;

    if (info->dump) {
      if (entry_count <= 8 ||
          i < 4 || i >= entry_count - 4) {

        print_spaces(info);
        printf("[%u]\n", i);

        info->depth++;

        PRINT_U(sample_count, info);
        PRINT_U(sample_offset, info);

        info->depth--;
      } else if (i == 4) {
        print_spaces(info);
        printf("[...]\n");
      }
    }

    entry[i].sample_count = sample_count;
    entry[i].sample_offset = sample_offset;
  }

  ctts = &p_box.mdia->minf.stbl.ctts;
  ctts->entry_count = entry_count;
  ctts->entry = entry;
free:
  if (ret)
    mem_free(entry);
exit:
  return ret;
}

static int
read_stsc(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int first_chunk;
  unsigned int samples_per_chunk;
  unsigned int sample_desc_index;
  struct stsc_entry * entry;
  struct box_stsc * stsc;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0 ||
      (ret = mem_alloc(&entry, entry_count * sizeof(* entry))) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&first_chunk, file)) != 0 ||
        (ret = read_u32(&samples_per_chunk, file)) != 0 ||
        (ret = read_u32(&sample_desc_index, file)) != 0)
      goto free;

    if (info->dump) {
      if (entry_count <= 8 ||
          i < 4 || i >= entry_count - 4) {

        print_spaces(info);
        printf("[%u]\n", i);

        info->depth++;

        PRINT_U(first_chunk, info);
        PRINT_U(samples_per_chunk, info);
        PRINT_U(sample_desc_index, info);

        info->depth--;
      } else if (i == 4) {
        print_spaces(info);
        printf("[...]\n");
      }
    }

    entry[i].first_chunk = first_chunk;
    entry[i].samples_per_chunk = samples_per_chunk;
    entry[i].sample_desc_index = sample_desc_index;
  }
  stsc = &p_box.mdia->minf.stbl.stsc;
  stsc->entry_count = entry_count;
  stsc->entry = entry;
free:
  if (ret)
    mem_free(entry);
exit:
  return ret;

}

static int
read_stco(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int chunk_offset;
  struct stco_entry * entry;
  struct box_stco * stco;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0 ||
      (ret = mem_alloc(&entry, entry_count * sizeof(* entry))) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&chunk_offset, file)) != 0)
      goto free;

    if (info->dump) {
      if (entry_count <= 10 ||
          i < 5 || i >= entry_count - 5) {
        print_spaces(info);
        printf("[%u] chunk_offset:            %u\n", i, chunk_offset);
      } else if (i == 5) {
        print_spaces(info);
        printf("[...]\n");
      }
    }

    entry[i].chunk_offset = chunk_offset;
  }
  stco = &p_box.mdia->minf.stbl.stco;
  stco->entry_count = entry_count;
  stco->entry = entry;
free:
  if (ret)
    mem_free(entry);
exit:
  return ret;
}

static int
read_stsz(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int sample_size;
  unsigned int sample_count;
  unsigned int entry_size;
  struct stsz_entry * entry;
  struct box_stsz * stsz;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&sample_size, file)) != 0 ||
      (ret = read_u32(&sample_count, file)) != 0)
    goto exit;

  if (info->dump) {
    PRINT_U(sample_size, info);
    PRINT_U(sample_count, info);
  }

  if (sample_size == 0) {

    if ((ret = mem_alloc(&entry, sample_count * sizeof(* entry))) != 0)
      goto exit;

    for (i = 0; i < sample_count; i++) {
      if ((ret = read_u32(&entry_size, file)) != 0)
        goto free;

      if (info->dump) {
        if (sample_count <= 10 ||
            i < 5 || i >= sample_count - 5) {
          print_spaces(info);
          printf("[%u] entry_size:            %u\n", i, entry_size);
        } else if (i == 5) {
          print_spaces(info);
          printf("[...]\n");
        }
      }

      entry[i].entry_size = entry_size;
    }
  } else {

    if ((ret = mem_alloc(&entry, sample_count * sizeof(* entry))) != 0)
      goto exit;

    for (i = 0; i < sample_count; i++)
      entry[i].entry_size = sample_size;
  }
  stsz = &p_box.mdia->minf.stbl.stsz;
  stsz->sample_size = sample_size;
  stsz->sample_count = sample_count;
  stsz->entry = entry;
free:
  if (ret)
    mem_free(entry);
exit:
  return ret;
}

static int
read_stss(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int entry_count;
  unsigned int sample_number;
  struct stss_entry * entry;
  struct box_stss * stss;
  unsigned int i;
  int ret;

  ret = 0;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&entry_count, file)) != 0 ||
      (ret = mem_alloc(&entry, entry_count * sizeof(* entry))) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&sample_number, file)) != 0)
      goto free;

    if (info->dump) {
      if (entry_count <= 10 ||
          i < 5 || i >= entry_count - 5) {
        print_spaces(info);
        printf("[%u] sample_number:            %u\n", i, sample_number);
      } else if (i == 5) {
        print_spaces(info);
        printf("[...]\n");
      }
    }

    entry[i].sample_number = sample_number;
  }
  stss = &p_box.mdia->minf.stbl.stss;
  stss->entry_count = entry_count;
  stss->entry = entry;
free:
  if (ret)
    mem_free(entry);
exit:
  return ret;
}

static int
read_sgpd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int grouping_type;
  unsigned int default_length;
  unsigned int entry_count;
  short roll_distance;
  unsigned int i;
  int ret;

  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&grouping_type, file)) != 0)
    return ret;

  if (info->dump)
    print_box("grouping_type", grouping_type, info);

  if (version == 1) {
    if ((ret = read_u32(&default_length, file)) != 0)
      return ret;

    if (info->dump)
      PRINT_U(default_length, info);
  }

  if ((ret = read_u32(&entry_count, file)) != 0)
    return ret;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_s16(&roll_distance, file)) != 0)
      return ret;

    if (info->dump)
      PRINT_S(roll_distance, info);
  }
  return 0;
}

static int
read_sbgp(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned int grouping_type;
  unsigned int entry_count;
  unsigned int sample_count;
  unsigned int group_desc_index;
  unsigned int i;
  int ret;

  (void) p_box;

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u32(&grouping_type, file)) != 0)
    return ret;

  if (info->dump)
    print_box("grouping_type", grouping_type, info);

  if ((ret = read_u32(&entry_count, file)) != 0)
    return ret;

  if (info->dump)
    PRINT_U(entry_count, info);

  for (i = 0; i < entry_count; i++) {
    if ((ret = read_u32(&sample_count, file)) != 0 ||
        (ret = read_u32(&group_desc_index, file)) != 0)
      return ret;

    if (info->dump) {
      PRINT_U(sample_count, info);
      PRINT_U(group_desc_index, info);
    }
  }
  return 0;
}

static int
read_stbl(FILE * file, struct box_info * info, box_t p_box) {
  struct box_func funcs[] = {
    {BOX_STSD, 0, BOX_QTY_1,      read_stsd},
    {BOX_STTS, 0, BOX_QTY_1,      read_stts},
    {BOX_CTTS, 0, BOX_QTY_0_OR_1, read_ctts},
    {BOX_STSC, 0, BOX_QTY_1,      read_stsc},
    {BOX_STCO, 0, BOX_QTY_1,      read_stco},
    {BOX_STSZ, 0, BOX_QTY_1,      read_stsz},
    {BOX_STSS, 0, BOX_QTY_0_OR_1, read_stss},
    {BOX_SGPD, 0, BOX_QTY_0_TO_N, read_sgpd},
    {BOX_SBGP, 0, BOX_QTY_0_TO_N, read_sbgp},
    {0, 0, 0, NULL}
  };
  return read_box(file, info, p_box, funcs);
}

static int
read_vmhd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  unsigned short graphicsmode;
  unsigned short opcolor[3];
  struct box_vmhd * vmhd;
  unsigned int i;
  int ret;

  ret = 0;

  if (p_box.mdia->hdlr.type != BOX_VIDE) {
    ret = ERR_UNK_HDLR_TYPE;
    goto exit;
  }

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_u16(&graphicsmode, file)) != 0 ||
      (ret = mem_alloc(&vmhd, sizeof(* vmhd))) != 0)
    goto exit;

  if (info->dump)
    PRINT_U(graphicsmode, info);

  for (i = 0; i < 3; i++)
    if ((ret = read_u16(&opcolor[i], file)) != 0)
      goto free;

  if (info->dump) {
    print_u("opcolor[0]", opcolor[0], info);
    print_u("opcolor[1]", opcolor[1], info);
    print_u("opcolor[2]", opcolor[2], info);
  }

  vmhd->graphicsmode = graphicsmode;

  for (i = 0; i < 3; i++)
    vmhd->opcolor[i] = opcolor[i];

  p_box.mdia->minf.hd.vmhd = vmhd;
free:
  if (ret)
    mem_free(vmhd);
exit:
  return ret;
}

static int
read_smhd(FILE * file, struct box_info * info, box_t p_box) {
  unsigned char version;
  unsigned int flags;
  short balance;
  struct box_smhd * smhd;
  int ret;

  ret = 0;

  if (p_box.mdia->hdlr.type != BOX_SOUN) {
    ret = ERR_UNK_HDLR_TYPE;
    goto exit;
  }

  if ((ret = read_ver(&version, &flags, file)) != 0 ||
      (ret = read_s16(&balance, file)) != 0 ||
      (ret = skip(file, 2)) != 0 || /* reserved */
      (ret = mem_alloc(&smhd, sizeof(* smhd))) != 0)
    goto exit;

  if (info->dump)
    PRINT_S_8(balance, info);

  smhd->balance = balance;

  p_box.mdia->minf.hd.smhd = smhd;
exit:
  return ret;
}

static int
read_minf(FILE * file, struct box_info * info, box_t p_box) {
  struct box_func funcs[] = {
    {BOX_DINF, 0, BOX_QTY_1,      read_dinf},
    {BOX_STBL, 0, BOX_QTY_1,      read_stbl},
    {BOX_VMHD, 0, BOX_QTY_0_OR_1, read_vmhd},
    {BOX_SMHD, 0, BOX_QTY_0_OR_1, read_smhd},
    {0, 0, 0, NULL}
  };
  return read_box(file, info, p_box, funcs);
}

static int
read_mdia(FILE * file, struct box_info * info, box_t p_box) {
  struct box_func funcs[] = {
    {BOX_MDHD, 0, BOX_QTY_1, read_mdhd},
    {BOX_HDLR, 0, BOX_QTY_1, read_hdlr},
    {BOX_MINF, 0, BOX_QTY_1, read_minf},
    {0, 0, 0, NULL}
  };
  box_t box;
  box.mdia = &p_box.trak->mdia;
  return read_box(file, info, box, funcs);
}

static void
init_trak(struct box_trak * trak) {
  trak->edts.elst.entry = NULL;
  trak->edts.elst.entry_count = 0;
  trak->mdia.hdlr.name = NULL;
  trak->mdia.hdlr.type = BOX_NIL;
  trak->mdia.minf.dinf.dref.entry = NULL;
  trak->mdia.minf.dinf.dref.entry_count = 0;
  trak->mdia.minf.stbl.stsd.entry.vide = NULL;
  trak->mdia.minf.stbl.stsd.entry_count = 0;
  trak->mdia.minf.stbl.stts.entry = NULL;
  trak->mdia.minf.stbl.ctts.entry = NULL;
  trak->mdia.minf.stbl.ctts.entry_count = 0;
  trak->mdia.minf.stbl.stsc.entry = NULL;
  trak->mdia.minf.stbl.stco.entry = NULL;
  trak->mdia.minf.stbl.stsz.entry = NULL;
  trak->mdia.minf.stbl.stss.entry = NULL;
  trak->mdia.minf.stbl.stss.entry_count = 0;
  trak->mdia.minf.hd.vmhd = NULL;
}

static void
free_trak(struct box_trak * trak) {
  unsigned int i;

  mem_free(trak->edts.elst.entry);
  mem_free(trak->mdia.hdlr.name);

  for (i = 0; i < trak->mdia.minf.dinf.dref.entry_count; i++)
    free_dref_entry(&trak->mdia.minf.dinf.dref.entry[i]);
  mem_free(trak->mdia.minf.dinf.dref.entry);

  if (trak->mdia.hdlr.type == BOX_VIDE) {
    for (i = 0; i < trak->mdia.minf.stbl.stsd.entry_count; i++)
      free_vide(&trak->mdia.minf.stbl.stsd.entry.vide[i]);
    mem_free(trak->mdia.minf.stbl.stsd.entry.vide);
  } else if (trak->mdia.hdlr.type == BOX_SOUN) {
    for (i = 0; i < trak->mdia.minf.stbl.stsd.entry_count; i++)
      free_soun(&trak->mdia.minf.stbl.stsd.entry.soun[i]);
    mem_free(trak->mdia.minf.stbl.stsd.entry.soun);
  }

  mem_free(trak->mdia.minf.stbl.stts.entry);
  mem_free(trak->mdia.minf.stbl.ctts.entry);
  mem_free(trak->mdia.minf.stbl.stsc.entry);
  mem_free(trak->mdia.minf.stbl.stco.entry);
  mem_free(trak->mdia.minf.stbl.stsz.entry);
  mem_free(trak->mdia.minf.stbl.stss.entry);
  mem_free(trak->mdia.minf.hd.vmhd);
}

static int
read_trak(FILE * file, struct box_info * info, box_t p_box) {
  struct box_func funcs[] = {
    {BOX_TKHD, 0, BOX_QTY_1,      read_tkhd},
    {BOX_EDTS, 0, BOX_QTY_0_OR_1, read_edts},
    {BOX_MDIA, 0, BOX_QTY_1,      read_mdia},
    {0, 0, 0, NULL}
  };
  struct box_moov * moov;
  struct box_trak * trak;
  box_t box;
  int ret;

  ret = 0;

  moov = p_box.moov;
  if ((ret = mem_realloc(&moov->trak,
                         (moov->trak_len + 1) * sizeof(* moov->trak))) != 0)
    goto exit;

  box.trak = trak = &moov->trak[moov->trak_len];

  init_trak(trak);

  if ((ret = read_box(file, info, box, funcs)) != 0)
    goto free;

  moov->trak_len++;
free:
  if (ret)
    free_trak(trak);
exit:
  return ret;
}

static int
read_moov(FILE * file, struct box_info * info, box_t p_box) {
  struct box_func funcs[] = {
    {BOX_MVHD, 0, BOX_QTY_1,      read_mvhd},
    {BOX_TRAK, 0, BOX_QTY_1_TO_N, read_trak},
    {BOX_IODS, 0, BOX_QTY_0_OR_1, read_iods},
    {BOX_UDTA, 0, BOX_QTY_0_TO_N, read_udta},
    {0, 0, 0, NULL}
  };
  box_t box;
  box.moov = &p_box.top->moov;
  return read_box(file, info, box, funcs);
}

static int
read_mdat(FILE * file, struct box_info * info, box_t p_box) {
  long pos;
  int ret;

  if ((ret = get_pos(&pos, file)) != 0 ||
      (ret = skip(file, info->size - (unsigned int) (pos - info->pos))) != 0)
    return ret;

  p_box.top->mdat.file = file;
  return 0;
}

static int
read_free(FILE * file, struct box_info * info, box_t p_box) {
  long pos;
  int ret;

  (void) p_box;

  if ((ret = get_pos(&pos, file)) != 0 ||
      (ret = skip(file, info->size - (unsigned int) (pos - info->pos))) != 0)
    return ret;
  return 0;
}

static int
fill_stbl(struct box_top * top) {
  struct box_moov * moov;
  struct box_stbl * stbl;
  struct box_stco * stco;
  struct box_stsc * stsc;
  struct box_stsz * stsz;
  unsigned int i;
  unsigned int j;
  unsigned int o; /* stco->entry[o] */
  unsigned int c; /* stsc->entry[c], index of chunk */
  unsigned int z; /* stsz->entry[z], index of sample */
  unsigned int first_chunk;
  unsigned int sample_offset;

  moov = &top->moov;

  for (i = 0; i < moov->trak_len; i++) {
    stbl = &moov->trak[i].mdia.minf.stbl;
    stco = &stbl->stco;
    stsc = &stbl->stsc;
    stsz = &stbl->stsz;
    first_chunk = stco->entry_count;
    z = stsz->sample_count;

    /* iterate each stsc entry reversely */
    for (c = stsc->entry_count; c--;) {

      /* iterate each chunk reversely */
      for (o = first_chunk; o-- > stsc->entry[c].first_chunk-1;) {

        sample_offset = stco->entry[o].chunk_offset;
        z -= stsc->entry[c].samples_per_chunk;

        /* iterate each sample */
        for (j = 0; j < stsc->entry[c].samples_per_chunk; j++) {
          stsz->entry[z+j].pos = sample_offset; /* set sample's pos */
          sample_offset += stsz->entry[z+j].entry_size;
        }

        /* set chunk's samples_per_chunk */
        stco->entry[o].samples_per_chunk = stsc->entry[c].samples_per_chunk;
      }
      first_chunk = stsc->entry[c].first_chunk-1;
    }
  }
  return 0;
}

static int
read_top(FILE * file, struct box_top * top) {
  struct box_info info;
  struct box_func funcs[] = {
    {BOX_FTYP, 0, BOX_QTY_1,      read_ftyp},
    {BOX_MOOV, 0, BOX_QTY_1,      read_moov},
    {BOX_MDAT, 0, BOX_QTY_1,      read_mdat},
    {BOX_FREE, 0, BOX_QTY_0_TO_N, read_free},
    {0, 0, 0, NULL}
  };
  box_t box;
  long size;
  int ret;

  if (fseek(file, 0, SEEK_END) == -1)
    return ERR_IO;

  if ((ret = get_pos(&size, file)) != 0)
    return ret;

  if (fseek(file, 0, SEEK_SET) == -1)
    return ERR_IO;

  info.pos = 0;
  info.size = (unsigned int) size;
  info.type = BOX_TOP;
  info.depth = 0;
  info.dump = 0;

  top->ftyp.c_brands = NULL;
  top->moov.iods = NULL;
  top->moov.trak = NULL;
  top->moov.trak_len = 0;

  box.top = top;
  if ((ret = read_box(file, &info, box, funcs)) != 0 ||
      (ret = fill_stbl(top)) != 0)
    return ret;
  return 0;
}

static void
free_top(struct box_top * top) {
  unsigned int i;

  for (i = 0; i < top->moov.trak_len; i++)
    free_trak(&top->moov.trak[i]);

  mem_free(top->moov.trak);
  mem_free(top->moov.iods);
  mem_free(top->ftyp.c_brands);
}

static int
open_file(FILE ** file_p, const char * fname) {
  FILE * file;

  file = fopen(fname, "rb");
  if (file == NULL)
    return ERR_IO;

  * file_p = file;
  return 0;
}

static void
close_file(FILE * file) {
  fclose(file);
}

static int
write_ary(void * ptr, size_t size, size_t len, FILE * file) {
  if (fwrite(ptr, size, len, file) != len)
    return ERR_IO;
  return 0;
}

static int
write_s16(short x, FILE * file) {
  unsigned char b16[2];

  b16[0] = (unsigned char) (x >> 8);
  b16[1] = (unsigned char) x;

  if (fwrite(b16, sizeof(b16), 1, file) != 1)
    return ERR_IO;
  return 0;
}

static int
write_s32(int x, FILE * file) {
  unsigned char b32[4];

  b32[0] = (unsigned char) (x >> 24);
  b32[1] = (unsigned char) (x >> 16);
  b32[2] = (unsigned char) (x >> 8);
  b32[3] = (unsigned char) x;

  if (fwrite(b32, sizeof(b32), 1, file) != 1)
    return ERR_IO;
  return 0;
}

static int
write_u8(unsigned char x, FILE * file) {
  if (fputc(x, file) == EOF)
    return ERR_IO;
  return 0;
}

static int
write_u16(unsigned short x, FILE * file) {
  unsigned char b16[2];

  b16[0] = (unsigned char) (x >> 8);
  b16[1] = (unsigned char) x;

  if (fwrite(b16, sizeof(b16), 1, file) != 1)
    return ERR_IO;
  return 0;
}

static int
write_u32(unsigned int x, FILE * file) {
  unsigned char b32[4];

  b32[0] = (unsigned char) (x >> 24);
  b32[1] = (unsigned char) (x >> 16);
  b32[2] = (unsigned char) (x >> 8);
  b32[3] = (unsigned char) x;

  if (fwrite(b32, sizeof(b32), 1, file) != 1)
    return ERR_IO;
  return 0;
}

static int
write_str(char * str, FILE * file) {
  if (fwrite(str, strlen(str) + 1, 1, file) != 1)
    return ERR_IO;
  return 0;
}

static int
write_ver(unsigned char version, unsigned int flags, FILE * file) {
  unsigned int b32;
  b32 = ((unsigned int) version << 24) | flags;
  return write_u32(b32, file);
}

static int
write_tag(unsigned char tag, unsigned int len, FILE * file) {
  unsigned int m; /* mask */
  unsigned int n; /* bit length of len */
  unsigned int b; /* byte */
  int ret;

  if ((ret = write_u8(tag, file)) != 0)
    return ret;

  if (len == 0)
    return write_u8(0, file);

  n = 32;
  m = 0x80000000;
  while ((len & m) == 0) {
    n--;
    m >>= 1;
  }
  if (n % 7) {
    n -= n % 7;
    b = (len >> n) | (n ? 0x80 : 0);
    if ((ret = write_u8((unsigned char) b, file)) != 0)
      return ret;
  }
  while (n >= 7) {
    n -= 7;
    b = ((len >> n) & 0x7f) | (n ? 0x80 : 0);
    if ((ret = write_u8((unsigned char) b, file)) != 0)
      return ret;
  }
  return 0;
}

static int
write_mat(int * matrix, FILE * file) {
  int i;
  int ret;

  for (i = 0; i < 9; i++)
    if ((ret = write_s32(matrix[i], file)) != 0)
      return ret;

  return 0;
}

static int
write_box(FILE * file, box_t box, unsigned int box_type,
          int (* func)(FILE * file, box_t box)) {
  long pos;
  long now;
  int ret;

  if ((ret = get_pos(&pos, file)) != 0 ||
      (ret = skip(file, 4)) != 0 ||
      (ret = write_u32(box_type, file)) != 0 ||
      (ret = func(file, box)) != 0 ||
      (ret = get_pos(&now, file)) != 0 ||
      (ret = set_pos(pos, file)) != 0 ||
      (ret = write_u32((unsigned int) (now - pos), file)) != 0 ||
      (ret = set_pos(now, file)) != 0)
    return ret;

  return 0;
}

static int
write_ftyp(FILE * file, box_t p_box) {
  struct box_ftyp * ftyp;
  unsigned int i;
  int ret;

  ftyp = &p_box.top->ftyp;
  if ((ret = write_u32(ftyp->m_brand, file)) != 0 ||
      (ret = write_u32(ftyp->m_version, file)) != 0)
    return ret;

  for (i = 0; i < ftyp->c_brands_len; i++)
    if ((ret = write_u32(ftyp->c_brands[i], file)) != 0)
      return ret;

  return 0;
}

static int
write_mvhd(FILE * file, box_t p_box) {
  struct box_mvhd * mvhd;
  int ret;

  mvhd = &p_box.moov->mvhd;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(mvhd->c_time, file)) != 0 ||
      (ret = write_u32(mvhd->m_time, file)) != 0 ||
      (ret = write_u32(mvhd->timescale, file)) != 0 ||
      (ret = write_u32(mvhd->duration, file)) != 0 ||
      (ret = write_s32(mvhd->rate, file)) != 0 ||
      (ret = write_s16(mvhd->volume, file)) != 0 ||
      (ret = skip(file, 2 + 4 * 2)) != 0 || /* reserved */
      (ret = write_mat(mvhd->matrix, file)) != 0 ||
      (ret = skip(file, 4 * 6)) != 0 || /* pre defined */
      (ret = write_u32(mvhd->next_track_id, file)) != 0)
    return ret;
  return 0;
}

static int
write_iods(FILE * file, box_t p_box) {
  unsigned short object_descr_id;
  struct box_iods * iods;
  struct initial_object_descr * iod;
  long pos;
  long now;
  int ret;

  iods = p_box.moov->iods;
  iod = &iods->iod;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = get_pos(&pos, file)) != 0 ||
      (ret = skip(file, 2)) != 0)
    return ret;

  object_descr_id = (unsigned short)
      ((iod->object_descr_id << 6) |
       (iod->url_flag << 5) |
       (iod->include_inline_profile_level_flag << 4) |
       0x0f);
  if ((ret = write_u16(object_descr_id, file)) != 0)
    return ret;

  if (iod->url_flag == 0) {
    if ((ret = write_u8(iod->od_profile_level_idc, file)) != 0 ||
        (ret = write_u8(iod->scene_profile_level_idc, file)) != 0 ||
        (ret = write_u8(iod->audio_profile_level_idc, file)) != 0 ||
        (ret = write_u8(iod->visual_profile_level_idc, file)) != 0 ||
        (ret = write_u8(iod->graphics_profile_levvel_idc, file)) != 0)
      return ret;
  }
  if ((ret = get_pos(&now, file)) != 0 ||
      (ret = set_pos(pos, file)) != 0 ||
      (ret = write_tag(TAG_MP4_IOD,
                       (unsigned int) (now - pos - 2), file)) != 0 ||
      (ret = set_pos(now, file)) != 0)
    return ret;
  return 0;
}

static int
write_tkhd(FILE * file, box_t p_box) {
  unsigned int flags;
  struct box_tkhd * tkhd;
  int ret;

  tkhd = &p_box.trak->tkhd;

  flags = (unsigned int) (tkhd->track_enabled |
                          (tkhd->track_in_movie << 1) |
                          (tkhd->track_in_preview << 2));

  if ((ret = write_ver(0, flags, file)) != 0 ||
      (ret = write_u32(tkhd->c_time, file)) != 0 ||
      (ret = write_u32(tkhd->m_time, file)) != 0 ||
      (ret = write_u32(tkhd->track_id, file)) != 0 ||
      (ret = skip(file, 4)) != 0 || /* reserved */
      (ret = write_u32(tkhd->duration, file)) != 0 ||
      (ret = skip(file, 4 * 2)) != 0 || /* reserved */
      (ret = write_s16(tkhd->layer, file)) != 0 ||
      (ret = write_s16(tkhd->alternate_group, file)) != 0 ||
      (ret = write_s16(tkhd->volume, file)) != 0 ||
      (ret = skip(file, 2)) != 0 || /* reserved */
      (ret = write_mat(tkhd->matrix, file)) != 0 ||
      (ret = write_u32(tkhd->width, file)) != 0 ||
      (ret = write_u32(tkhd->height, file)) != 0)
    return ret;
  return 0;
}

static int
write_mdhd(FILE * file, box_t p_box) {
  unsigned short lang_pack;
  struct box_mdhd * mdhd;
  int ret;

  mdhd = &p_box.mdia->mdhd;

  lang_pack = (unsigned short) (((mdhd->lang[0] - 0x60) << 10) |
                                ((mdhd->lang[1] - 0x60) << 5) |
                                (mdhd->lang[2] - 0x60));

  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(mdhd->c_time, file)) != 0 ||
      (ret = write_u32(mdhd->m_time, file)) != 0 ||
      (ret = write_u32(mdhd->timescale, file)) != 0 ||
      (ret = write_u32(mdhd->duration, file)) != 0 ||
      (ret = write_u16(lang_pack, file)) != 0 ||
      (ret = write_u16(0, file)) != 0) /* pre defined */
    return ret;
  return 0;
}

static int
write_hdlr(FILE * file, box_t p_box) {
  struct box_hdlr * hdlr;
  int ret;

  hdlr = &p_box.mdia->hdlr;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = skip(file, 4)) != 0 || /* pre defined */
      (ret = write_u32(hdlr->type, file)) != 0 ||
      (ret = skip(file, 4 * 3)) != 0 || /* reserved */
      (ret = write_str(hdlr->name, file)) != 0)
    return ret;
  return 0;
}

static int
write_dref_entry(FILE * file, box_t box) {
  unsigned int flags;
  struct box_dref_entry * entry;
  int ret;

  entry = box.dref_entry;

  flags = entry->self_contained;

  if ((ret = write_ver(0, flags, file)) != 0)
    return ret;

  if (entry->type == BOX_URL) {

    if (entry->self_contained == 0)
      if ((ret = write_str(entry->location, file)) != 0)
        return ret;
  } else if (entry->type == BOX_URN) {

    if ((ret = write_str(entry->name, file)) != 0 ||
        (ret = write_str(entry->location, file)) != 0)
      return ret;
  } else {
    return ERR_UNK_BOX;
  }
  return 0;
}

static int
write_dref(FILE * file, box_t p_box) {
  struct box_dref * dref;
  box_t box;
  unsigned int i;
  int ret;

  dref = &p_box.dinf->dref;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(dref->entry_count, file)) != 0)
    return ret;

  for (i = 0; i < dref->entry_count; i++) {
    box.dref_entry = &dref->entry[i];
    if ((ret = write_box(file, box, dref->entry[i].type,
                         write_dref_entry)) != 0)
      return ret;
  }
  return 0;
}

static int
write_dinf(FILE * file, box_t p_box) {
  box_t box;
  int ret;

  box.dinf = &p_box.mdia->minf.dinf;
  if ((ret = write_box(file, box, BOX_DREF, write_dref)) != 0)
    return ret;
  return 0;
}

static int
write_bits_init(struct bits * b, unsigned char * bytes, unsigned int size) {
  if (size == 0)
    return ERR_EMPTY_BITS;
  b->size = size;
  b->bytes = bytes;
  b->buf = 0;
  b->i = 0;
  b->pos = 0;
  return 0;
}

static int
write_bits_resize(struct bits * b) {
  int ret;

  if ((ret = mem_realloc(&b->bytes, b->size << 1)) != 0)
    return ret;
  b->size <<= 1;
  return 0;
}

static int
write_bits_flush(struct bits * b) {
  int ret;

  if (b->pos == 0)
    return 0;
  if (b->i >= b->size)
    if ((ret = write_bits_resize(b)) != 0)
      return ret;
  b->bytes[b->i++] = (b->buf << (8 - b->pos)) & 0xff;
  b->pos = 0;
  b->buf = 0;
  return 0;
}

static int
write_code(unsigned int n, struct bits * bits) {
  unsigned int z; /* number of leading zeros */
  unsigned int j;

  unsigned char * b;
  unsigned int size;
  unsigned int i;
  unsigned int pos;
  unsigned int u;

  int ret;

  b    = bits->bytes;
  size = bits->size;
  i    = bits->i;
  pos  = bits->pos;
  u    = bits->buf;

  if (n == 0xffffffff) {

    /* step 1: add by 1 */
    n = 0;

    /* step 2: count leading zeros */
    z = 32;

  } else {

    /* step 1: add by 1 */
    n++;

    /* step 2: count leading zeros */
    z = 0;
    j = n;
    while (j > 1) {
      j >>= 1;
      z++;
    }

    /* step 3: remove first non-zero bit */
    n &= ~((unsigned int) 1 << z);
  }

  while (i + ((pos + z + z + 1 - 1) >> 3) > size) {
    if ((ret = write_bits_resize(bits)) != 0)
      return ret;
    b    = bits->bytes;
    size = bits->size;
  }

  /* write zeros */
  if (z > 8 - pos) {
    u <<= 8 - pos;
    pos = z - (8 - pos);
    while (pos > 8) {
      pos -= 8;
      b[i++] = u & 0xff;
      u = 0;
    }
    if (pos) {
      b[i++] = u & 0xff;
      u = 0;
    }
  } else {
    u <<= z;
    pos += z;
  }

  /* first non-zero bit */
  if (pos == 8) {
    pos = 1;
    b[i++] = u & 0xff;
    u = 1;
  } else {
    u <<= 1;
    u |= 1;
    pos += 1;
  }

  /* last n bit */
  if (z > 8 - pos) {
    u <<= 8 - pos;
    pos = z - (8 - pos);
    u |= n >> pos;
    while (pos > 8) {
      pos -= 8;
      b[i++] = u & 0xff;
      u = n >> pos;
    }
    if (pos) {
      b[i++] = u & 0xff;
      u = n;
    }
  } else {
    u <<= z;
    u |= n;
    pos += z;
  }

  bits->buf = u;
  bits->pos = pos;
  bits->i = i;
  return 0;
}

static int
write_bit(unsigned char n, struct bits * bits) {
  unsigned char * b;
  unsigned int size;
  unsigned int i;
  unsigned int pos;
  unsigned int u;

  int ret;

  b    = bits->bytes;
  size = bits->size;
  i    = bits->i;
  pos  = bits->pos;
  u    = bits->buf;

  if (n > 1)
    return ERR_WRITE_BIT;

  if (pos == 8) {
    while (i >= size) {
      if ((ret = write_bits_resize(bits)) != 0)
        return ret;
      b    = bits->bytes;
      size = bits->size;
    }
    pos = 1;
    b[i++] = u & 0xff;
    u = n;
  } else {
    u <<= 1;
    u |= n;
    pos += 1;
  }

  bits->buf = u;
  bits->pos = pos;
  bits->i = i;
  return 0;
}

static int
write_bits(unsigned int n, unsigned char w, /* width */
           struct bits * bits) {
  unsigned char * b;
  unsigned int size;
  unsigned int i;
  unsigned int pos;
  unsigned int u;

  int ret;

  b    = bits->bytes;
  size = bits->size;
  i    = bits->i;
  pos  = bits->pos;
  u    = bits->buf;

  /* width valid range: 1~32 */
  if (w < 1 || w > 32)
    return ERR_WRITE_BITS;

  while (i + ((pos + w - 1) >> 3) > size) {
    if ((ret = write_bits_resize(bits)) != 0)
      return ret;
    b    = bits->bytes;
    size = bits->size;
  }

  if (w > 8 - pos) {
    u <<= 8 - pos;
    pos = w - (8 - pos);
    u |= n >> pos;
    while (pos > 8) {
      pos -= 8;
      b[i++] = u & 0xff;
      u = n >> pos;
    }
    if (pos) {
      b[i++] = u & 0xff;
      u = n;
    }
  } else {
    u <<= w;
    u |= n;
    pos += w;
  }

  bits->buf = u;
  bits->pos = pos;
  bits->i = i;
  return 0;
}

static int
write_vui_para(struct vui * vui, struct bits * bits) {
  int ret;

  ret = 0;

  if ((ret = write_bit(vui->aspect_ratio_info_present_flag, bits)) != 0)
    goto exit;

  if (vui->aspect_ratio_info_present_flag)
    if ((ret = write_bits(vui->aspect_ratio_idc, 8, bits)) != 0)
      goto exit;

  if ((ret = write_bit(vui->overscan_info_present_flag, bits)) != 0)
    goto exit;

  if ((ret = write_bit(vui->video_signal_type_present_flag, bits)) != 0)
    goto exit;

  if (vui->video_signal_type_present_flag) {
    if ((ret = write_bits(vui->video_format, 3, bits)) != 0 ||
        (ret = write_bit(vui->video_full_range_flag, bits)) != 0 ||
        (ret = write_bit(vui->colour_description_present_flag, bits)) != 0)
      goto exit;

    if (vui->colour_description_present_flag)
      if ((ret = write_bits(vui->colour_primaries, 8, bits)) != 0 ||
          (ret = write_bits(vui->transfer_characteristics, 8, bits)) != 0 ||
          (ret = write_bits(vui->matrix_coefficients, 8, bits)) != 0)
        goto exit;
  }

  if ((ret = write_bit(vui->chroma_loc_info_present_flag, bits)) != 0)
    goto exit;

  if ((ret = write_bit(vui->timing_info_present_flag, bits)) != 0)
    goto exit;

  if (vui->timing_info_present_flag)
    if ((ret = write_bits(vui->num_units_in_tick, 32, bits)) != 0 ||
        (ret = write_bits(vui->time_scale, 32, bits)) != 0 ||
        (ret = write_bit(vui->fixed_frame_rate_flag, bits)) != 0)
      goto exit;

  if ((ret = write_bit(vui->nal_hrd_para_present_flag, bits)) != 0)
    goto exit;

  if ((ret = write_bit(vui->vcl_hrd_para_present_flag, bits)) != 0)
    goto exit;

  if ((ret = write_bit(vui->pic_struct_present_flag, bits)) != 0 ||
      (ret = write_bit(vui->bitstream_restriction_flag, bits)) != 0)
    goto exit;

  if (vui->bitstream_restriction_flag)
    if ((ret = write_bit(vui->motion_vectors_over_pic_boundaries_flag,
                         bits)) != 0 ||
        (ret = write_code(vui->max_bytes_per_pic_denom, bits)) != 0 ||
        (ret = write_code(vui->max_bits_per_mb_denom, bits)) != 0 ||
        (ret = write_code(vui->log2_max_mv_length_horizontal, bits)) != 0 ||
        (ret = write_code(vui->log2_max_mv_length_vertical, bits)) != 0 ||
        (ret = write_code(vui->num_reorder_frames, bits)) != 0 ||
        (ret = write_code(vui->max_dec_frame_buffering, bits)) != 0)
      goto exit;
exit:
  return ret;
}

static int
write_nalu(union nalu arg_nalu, FILE * file) {
  unsigned char nal_ref_idc;
  unsigned char nal_unit_type;
  struct bits bits;
  unsigned char * nalu;
  unsigned int size;
  unsigned int i;
  long pos;
  long now;
  int ret;

  nal_ref_idc = arg_nalu.sps->nal_ref_idc;
  nal_unit_type = arg_nalu.sps->nal_unit_type;

  size = 1;

  if ((ret = mem_alloc(&nalu, size)) != 0)
    goto exit;

  if ((ret = write_bits_init(&bits, nalu, size)) != 0 ||
      (ret = write_bits(nal_ref_idc, 3, &bits)) != 0 ||
      (ret = write_bits(nal_unit_type, 5, &bits)) != 0)
    goto free;

  if (nal_unit_type == NAL_SPS) {
    unsigned char profile_comp;
    struct sps * sps;

    sps = arg_nalu.sps;

    profile_comp = (unsigned char) ((sps->c_set0_flag << 7) |
                                    (sps->c_set1_flag << 6) |
                                    (sps->c_set2_flag << 5) |
                                    (sps->c_set3_flag << 4) |
                                    (sps->c_set4_flag << 3) |
                                    (sps->c_set5_flag << 2));

    if ((ret = write_bits(sps->profile_idc, 8, &bits)) != 0 ||
        (ret = write_bits(profile_comp, 8, &bits)) != 0 ||
        (ret = write_bits(sps->level_idc, 8, &bits)) != 0 ||
        (ret = write_code(sps->seq_para_set_id, &bits)) != 0 ||
        (ret = write_code(sps->log2_max_frame_num_minus4, &bits)) != 0 ||
        (ret = write_code(sps->pic_order_cnt_type, &bits)) != 0)
      goto free;

    if (sps->pic_order_cnt_type == 0)
      if ((ret = write_code(sps->log2_max_pic_order_cnt_lsb_minus4,
                            &bits)) != 0)
        goto free;

    if ((ret = write_code(sps->max_num_ref_frames, &bits)) != 0 ||
        (ret = write_bit(sps->gaps_in_frame_num_value_allowed_flag,
                         &bits)) != 0 ||
        (ret = write_code(sps->pic_width_in_mbs_minus_1, &bits)) != 0 ||
        (ret = write_code(sps->pic_height_in_mbs_minus_1, &bits)) != 0 ||
        (ret = write_bit(sps->frame_mbs_only_flag, &bits)) != 0)
      goto free;

    if (!sps->frame_mbs_only_flag)
      if ((ret = write_bit(sps->mb_adaptive_frame_field_flag, &bits)) != 0)
        goto free;

    if ((ret = write_bit(sps->direct_8x8_inference_flag, &bits)) != 0 ||
        (ret = write_bit(sps->frame_cropping_flag, &bits)) != 0)
      goto free;

    if (sps->frame_cropping_flag) {
      if ((ret = write_code(sps->frame_crop_left_offset, &bits)) != 0 ||
          (ret = write_code(sps->frame_crop_right_offset, &bits)) != 0 ||
          (ret = write_code(sps->frame_crop_top_offset, &bits)) != 0 ||
          (ret = write_code(sps->frame_crop_bottom_offset, &bits)) != 0)
        goto free;
    }

    if ((ret = write_bit(sps->vui_para_present_flag, &bits)) != 0)
      goto free;

    if (sps->vui_para_present_flag)
      if ((ret = write_vui_para(&sps->vui, &bits)) != 0)
        goto free;
  } else if (nal_unit_type == NAL_PPS) {
    struct pps * pps;

    pps = arg_nalu.pps;
    if ((ret = write_code(pps->pic_para_set_id, &bits)) != 0 ||
        (ret = write_code(pps->seq_para_set_id, &bits)) != 0 ||
        (ret = write_bit(pps->entropy_coding_mode_flag, &bits)) != 0 ||
        (ret = write_bit(pps->pic_order_present_flag, &bits)) != 0 ||
        (ret = write_code(pps->num_slice_groups_minus1, &bits)) != 0 ||
        (ret = write_code(pps->num_ref_idx_10_active_minus1, &bits)) != 0 ||
        (ret = write_code(pps->num_ref_idx_11_active_minus1, &bits)) != 0 ||
        (ret = write_bit(pps->weighted_pred_flag, &bits)) != 0 ||
        (ret = write_bits(pps->weighted_bipred_idc, 2, &bits)) != 0 ||
        (ret = write_code(pps->pic_init_qp_minus26, &bits)) != 0 ||
        (ret = write_code(pps->pic_init_qs_minus26, &bits)) != 0 ||
        (ret = write_code(pps->chroma_qp_index_offset, &bits)) != 0 ||
        (ret = write_bit(pps->deblocking_filter_control_present_flag,
                         &bits)) != 0 ||
        (ret = write_bit(pps->constrained_intra_pred_flag, &bits)) != 0 ||
        (ret = write_bit(pps->redundant_pic_cnt_present_flag, &bits)) != 0)
      goto free;
  } else {
    ret = ERR_UNK_NAL_UNIT_TYPE;
    goto free;
  }

  if ((ret = write_bit(1, &bits)) != 0 || /* rbsp_stop_one_bit */
      (ret = write_bits_flush(&bits)) != 0) /* rbsp_alignment_zero_bit */
    goto free;

  if ((ret = get_pos(&pos, file)) != 0 ||
      (ret = skip(file, 2)) != 0)
    goto free;

  size = bits.i;
  nalu = bits.bytes;

  if ((ret = write_u8(nalu[0], file)) != 0)
    goto free;

  for (i = 1; i < size;) {
    if (i + 2 < size && !nalu[i] && !nalu[i+1] && !(nalu[i+2] & 0xfc)) {
      if ((ret = write_u8(nalu[i++], file)) != 0 ||
          (ret = write_u8(nalu[i++], file)) != 0 ||
          (ret = write_u8(0x03, file)) != 0 ||
          (ret = write_u8(nalu[i++], file)) != 0)
        goto free;
    } else {
      if ((ret = write_u8(nalu[i++], file)) != 0)
        goto free;
    }
  }
  if ((ret = get_pos(&now, file)) != 0 ||
      (ret = set_pos(pos, file)) != 0 ||
      (ret = write_u16((unsigned short) (now - pos - 2), file)) != 0 ||
      (ret = set_pos(now, file)) != 0)
    goto free;
free:
  nalu = bits.bytes;
  mem_free(nalu);
exit:
  return ret;
}

static int
write_avcc(FILE * file, box_t p_box) {
  unsigned char profile_comp;
  unsigned int i;
  struct box_avcc * avcc;
  union nalu nalu;
  int ret;

  avcc = &p_box.vide->avcc;

  profile_comp = (unsigned char) ((avcc->c_set0_flag << 7) |
                                  (avcc->c_set1_flag << 6) |
                                  (avcc->c_set2_flag << 5) |
                                  (avcc->c_set3_flag << 4) |
                                  (avcc->c_set4_flag << 3) |
                                  (avcc->c_set5_flag << 2));

  if ((ret = write_u8(avcc->conf_version, file)) != 0 ||
      (ret = write_u8(avcc->profile_idc, file)) != 0 ||
      (ret = write_u8(profile_comp, file)) != 0 ||
      (ret = write_u8(avcc->level_idc, file)) != 0 ||
      (ret = write_u8(avcc->len_size_minus_one | 0xfc, file)) != 0 ||
      (ret = write_u8(avcc->num_of_sps | 0xe0, file)) != 0)
    return ret;

  for (i = 0; i < avcc->num_of_sps; i++) {
    nalu.sps = &avcc->sps.sps[i];
    if ((ret = write_nalu(nalu, file)) != 0)
      return ret;
  }

  if ((ret = write_u8(avcc->num_of_pps, file)) != 0)
    return ret;

  for (i = 0; i < avcc->num_of_pps; i++) {
    nalu.pps = &avcc->pps.pps[i];
    if ((ret = write_nalu(nalu, file)) != 0)
      return ret;
  }
  return 0;
}

static int
write_vide(FILE * file, box_t box) {
  unsigned char len;
  struct box_vide * vide;
  int ret;

  vide = box.vide;

  len = (unsigned char) strlen(vide->compressorname);

  if ((ret = skip(file, 6)) != 0 || /* reserved */
      (ret = write_u16(vide->dref_index, file)) != 0 ||
      (ret = skip(file, 2 + 2 + 4 * 3)) != 0 || /* pre / reserved / pre */
      (ret = write_u16(vide->width, file)) != 0 ||
      (ret = write_u16(vide->height, file)) != 0 ||
      (ret = write_u32(vide->h_rez, file)) != 0 ||
      (ret = write_u32(vide->v_rez, file)) != 0 ||
      (ret = skip(file, 4)) != 0 || /* reserved */
      (ret = write_u16(vide->frame_count, file)) != 0 ||
      (ret = write_u8(len, file)) != 0 ||
      (ret = write_ary(vide->compressorname,
                       sizeof(vide->compressorname) - 1, 1, file)) != 0 ||
      (ret = write_u16(vide->depth, file)) != 0 ||
      (ret = write_s16((short) -1, file)) != 0 || /* pre defined */
      (ret = write_box(file, box, BOX_AVCC, write_avcc)) != 0)
    return ret;
  return 0;
}

static int
write_esds(FILE * file, box_t p_box) {
  struct box_esds * esds;
  struct es_descr * es;
  struct decoder_config_descr * dec;
  struct sl_config_descr * sl;
  struct audio_specific_config * audio;

  struct bits bits;
  unsigned char * bytes;
  unsigned char codec;
  unsigned int buffer_size_db;
  unsigned int i;
  long pos_es;
  long pos_dec;
  long pos_audio;
  long pos_sl;
  long now;
  int ret;

  esds = &p_box.soun->esds;
  es = &esds->es;
  dec = &es->dec_conf;
  audio = &dec->audio;
  sl = &es->sl_conf;

  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = get_pos(&pos_es, file)) != 0 ||
      (ret = skip(file, 2)) != 0)
    return ret;

  if ((ret = write_u16(es->es_id, file)) != 0 ||
      (ret = write_u8(es->es_flags, file)) != 0 ||
      (ret = get_pos(&pos_dec, file)) != 0 ||
      (ret = skip(file, 2)) != 0)
    return ret;

  buffer_size_db = ((unsigned int) (dec->stream_type << 26) |
                    (unsigned int) (dec->up_stream << 25) |
                    (1 << 24) | /* reserved */
                    dec->buffer_size_db);

  if ((ret = write_u8(dec->object_type_idc, file)) != 0 ||
      (ret = id_to_codec(&codec, dec->object_type_idc)) != 0 ||
      (ret = write_u32(buffer_size_db, file)) != 0 ||
      (ret = write_u32(dec->max_bitrate, file)) != 0 ||
      (ret = write_u32(dec->avg_bitrate, file)) != 0 ||
      (ret = get_pos(&pos_audio, file)) != 0 ||
      (ret = skip(file, 2)) != 0)
    return ret;

  if (codec == CODEC_AAC) {

    if ((ret = mem_alloc(&bytes, audio->tag_len)) != 0)
      return ret;

    for (i = 0; i < audio->tag_len; i++)
      bytes[i] = 0;

    if ((ret = write_bits_init(&bits, bytes, audio->tag_len)) != 0)
      goto free;

    if (audio->audio_object_type >= 0x20) {
      if ((ret = write_bits(0x1f, 5, &bits)) != 0 ||
          (ret = write_bits((unsigned int)
                            audio->audio_object_type - 0x20, 6, &bits)) != 0)
        goto free;
    } else {
      if ((ret = write_bits(audio->audio_object_type, 5, &bits)) != 0)
        goto free;
    }

    if ((ret = write_bits(audio->sampling_frequency_index, 4, &bits)) != 0)
      goto free;

    if (audio->sampling_frequency_index == 0xf)
      if ((ret = write_bits(audio->sampling_frequency, 24, &bits)) != 0)
        goto free;

    if ((ret = write_bits(audio->channel_configuration, 4, &bits)) != 0 ||
        (ret = write_bits_flush(&bits)) != 0 ||
        (ret = write_ary(bits.bytes, bits.size, 1, file)) != 0)
      goto free;
free:
    mem_free(bits.bytes);
    if (ret)
      return ret;
  } else {
    return ERR_UNK_CODEC;
  }

  if ((ret = get_pos(&now, file)) != 0 ||
      (ret = set_pos(pos_audio, file)) != 0 ||
      (ret = write_tag(TAG_DEC_SPECIFIC_INFO,
                       (unsigned int) (now - pos_audio - 2), file)) != 0 ||
      (ret = set_pos(pos_dec, file)) != 0 ||
      (ret = write_tag(TAG_DECODER_CONFIG_DESCR,
                       (unsigned int) (now - pos_dec - 2), file)) != 0 ||
      (ret = set_pos(now, file)) != 0 ||
      (ret = skip(file, 2)) != 0)
    return ret;

  pos_sl = now;

  if ((ret = write_u8(sl->predefined, file)) != 0 ||
      (ret = get_pos(&now, file)) != 0 ||
      (ret = set_pos(pos_sl, file)) != 0 ||
      (ret = write_tag(TAG_SL_CONFIG_DESCR,
                       (unsigned int) (now - pos_sl - 2), file)) != 0 ||
      (ret = set_pos(pos_es, file)) != 0 ||
      (ret = write_tag(TAG_ES_DESCR,
                       (unsigned int) (now - pos_es - 2), file)) != 0 ||
      (ret = set_pos(now, file)) != 0)
    return ret;
  return 0;
}

static int
write_soun(FILE * file, box_t box) {
  struct box_soun * soun;
  int ret;

  soun = box.soun;

  if ((ret = skip(file, 6)) != 0 || /* reserved */
      (ret = write_u16(soun->dref_index, file)) != 0 ||
      (ret = skip(file, 4 * 2)) != 0 || /* reserved */
      (ret = write_u16(soun->channelcount, file)) != 0 ||
      (ret = write_u16(soun->samplesize, file)) != 0 ||
      (ret = skip(file, 2 + 2)) != 0 || /* pre defined / reserved */
      (ret = write_u32(soun->samplerate, file)) != 0 ||
      (ret = write_box(file, box, BOX_ESDS, write_esds)) != 0)
    return ret;
  return 0;
}

static int
write_stsd(FILE * file, box_t p_box) {
  struct box_stsd * stsd;
  unsigned int type;
  unsigned int i;
  box_t box;
  int ret;

  stsd = &p_box.mdia->minf.stbl.stsd;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(stsd->entry_count, file)) != 0)
    return ret;

  type = p_box.mdia->hdlr.type;
  for (i = 0; i < stsd->entry_count; i++) {
    if (type == BOX_VIDE) {
      box.vide = &stsd->entry.vide[i];
      if ((ret = write_box(file, box, BOX_AVC1, write_vide)) != 0)
        return ret;
    } else if (type == BOX_SOUN) {
      box.soun = &stsd->entry.soun[i];
      if ((ret = write_box(file, box, BOX_MP4A, write_soun)) != 0)
        return ret;
    } else {
      return ERR_UNK_HDLR_TYPE;
    }
  }
  return 0;
}

static int
write_stts(FILE * file, box_t p_box) {
  struct box_stts * stts;
  unsigned int i;
  int ret;

  stts = &p_box.mdia->minf.stbl.stts;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(stts->entry_count, file)) != 0)
    return ret;

  for (i = 0; i < stts->entry_count; i++)
    if ((ret = write_u32(stts->entry[i].sample_count, file)) != 0 ||
        (ret = write_u32(stts->entry[i].sample_delta, file)) != 0)
      return ret;
  return 0;
}

static int
write_ctts(FILE * file, box_t p_box) {
  struct box_ctts * ctts;
  unsigned int i;
  int ret;

  ctts = &p_box.mdia->minf.stbl.ctts;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(ctts->entry_count, file)) != 0)
    return ret;

  for (i = 0; i < ctts->entry_count; i++)
    if ((ret = write_u32(ctts->entry[i].sample_count, file)) != 0 ||
        (ret = write_u32(ctts->entry[i].sample_offset, file)) != 0)
      return ret;
  return 0;
}

static int
write_stsc(FILE * file, box_t p_box) {
  struct box_stsc * stsc;
  unsigned int i;
  int ret;

  stsc = &p_box.mdia->minf.stbl.stsc;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(stsc->entry_count, file)) != 0)
    return ret;

  for (i = 0; i < stsc->entry_count; i++)
    if ((ret = write_u32(stsc->entry[i].first_chunk, file)) != 0 ||
        (ret = write_u32(stsc->entry[i].samples_per_chunk, file)) != 0 ||
        (ret = write_u32(stsc->entry[i].sample_desc_index, file)) != 0)
      return ret;
  return 0;
}

static int
write_stco(FILE * file, box_t p_box) {
  struct box_stco * stco;
  int ret;

  stco = &p_box.mdia->minf.stbl.stco;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(stco->entry_count, file)) != 0 ||
      (ret = get_pos(&stco->pos, file)) != 0 ||
      (ret = skip(file, stco->entry_count * 4)) != 0)
    return ret;
  return 0;
}

static int
write_stsz(FILE * file, box_t p_box) {
  struct box_stsz * stsz;
  unsigned int i;
  int ret;

  stsz = &p_box.mdia->minf.stbl.stsz;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(stsz->sample_size, file)) != 0 ||
      (ret = write_u32(stsz->sample_count, file)) != 0)
    return ret;

  if (stsz->sample_size == 0)
    for (i = 0; i < stsz->sample_count; i++)
      if ((ret = write_u32(stsz->entry[i].entry_size, file)) != 0)
        return ret;
  return 0;
}

static int
write_stss(FILE * file, box_t p_box) {
  struct box_stss * stss;
  unsigned int i;
  int ret;

  stss = &p_box.mdia->minf.stbl.stss;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(stss->entry_count, file)) != 0)
    return ret;

  for (i = 0; i < stss->entry_count; i++)
    if ((ret = write_u32(stss->entry[i].sample_number, file)) != 0)
      return ret;
  return 0;
}

static int
write_stbl(FILE * file, box_t p_box) {
  struct box_stbl * stbl;
  int ret;

  stbl = &p_box.mdia->minf.stbl;

  if ((ret = write_box(file, p_box, BOX_STSD, write_stsd)) != 0 ||
      (ret = write_box(file, p_box, BOX_STTS, write_stts)) != 0)
    return ret;

  if (stbl->ctts.entry_count)
    if ((ret = write_box(file, p_box, BOX_CTTS, write_ctts)) != 0)
      return ret;

  if ((ret = write_box(file, p_box, BOX_STSC, write_stsc)) != 0 ||
      (ret = write_box(file, p_box, BOX_STCO, write_stco)) != 0 ||
      (ret = write_box(file, p_box, BOX_STSZ, write_stsz)) != 0)
    return ret;

  if (stbl->stss.entry_count)
    if ((ret = write_box(file, p_box, BOX_STSS, write_stss)) != 0)
      return ret;
  return 0;
}

static int
write_vmhd(FILE * file, box_t p_box) {
  struct box_vmhd * vmhd;
  unsigned int i;
  int ret;

  vmhd = p_box.mdia->minf.hd.vmhd;

  if ((ret = write_ver(0, 1, file)) != 0 ||
      (ret = write_u16(vmhd->graphicsmode, file)) != 0)
    return ret;

  for (i = 0; i < 3; i++)
    if ((ret = write_u16(vmhd->opcolor[i], file)) != 0)
      return ret;
  return 0;
}

static int
write_smhd(FILE * file, box_t p_box) {
  struct box_smhd * smhd;
  int ret;

  smhd = p_box.mdia->minf.hd.smhd;

  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_s16(smhd->balance, file)) != 0 ||
      (ret = write_u16(0, file)) != 0) /* reserved */
    return ret;
  return 0;
}

static int
write_minf(FILE * file, box_t p_box) {
  unsigned int type;
  int ret;

  if ((ret = write_box(file, p_box, BOX_DINF, write_dinf)) != 0 ||
      (ret = write_box(file, p_box, BOX_STBL, write_stbl)) != 0)
    return ret;

  type = p_box.mdia->hdlr.type;

  if (type == BOX_VIDE) {
    if ((ret = write_box(file, p_box, BOX_VMHD, write_vmhd)) != 0)
      return ret;
  } else if (type == BOX_SOUN) {
    if ((ret = write_box(file, p_box, BOX_SMHD, write_smhd)) != 0)
      return ret;
  } else {
    return ERR_UNK_HDLR_TYPE;
  }
  return 0;
}

static int
write_elst(FILE * file, box_t p_box) {
  struct box_elst * elst;
  unsigned int i;
  int ret;

  elst = &p_box.edts->elst;
  if ((ret = write_ver(0, 0, file)) != 0 ||
      (ret = write_u32(elst->entry_count, file)) != 0)
    return ret;

  for (i = 0; i < elst->entry_count; i++)
    if ((ret = write_u32(elst->entry[i].segment_duration, file)) != 0 ||
        (ret = write_s32(elst->entry[i].media_time, file)) != 0 ||
        (ret = write_s16(elst->entry[i].media_rate_integer, file)) != 0 ||
        (ret = write_s16(elst->entry[i].media_rate_fraction, file)) != 0)
      return ret;
  return 0;
}

static int
write_edts(FILE * file, box_t p_box) {
  box_t box;
  box.edts = &p_box.trak->edts;
  return write_box(file, box, BOX_ELST, write_elst);
}

static int
write_mdia(FILE * file, box_t p_box) {
  box_t box;
  int ret;

  box.mdia = &p_box.trak->mdia;
  if ((ret = write_box(file, box, BOX_MDHD, write_mdhd)) != 0 ||
      (ret = write_box(file, box, BOX_HDLR, write_hdlr)) != 0 ||
      (ret = write_box(file, box, BOX_MINF, write_minf)) != 0)
    return ret;
  return 0;
}

static int
write_trak(FILE * file, box_t box) {
  struct box_trak * trak;
  int ret;

  trak = box.trak;

  if ((ret = write_box(file, box, BOX_TKHD, write_tkhd)) != 0)
    return ret;

  if (trak->edts.elst.entry_count)
    if ((ret = write_box(file, box, BOX_EDTS, write_edts)) != 0)
      return ret;

  if ((ret = write_box(file, box, BOX_MDIA, write_mdia)) != 0)
    return ret;
  return 0;
}

static int
write_moov(FILE * file, box_t p_box) {
  struct box_moov * moov;
  box_t box;
  unsigned int i;
  int ret;

  box.moov = moov = &p_box.top->moov;
  if ((ret = write_box(file, box, BOX_MVHD, write_mvhd)) != 0)
    return ret;

  if (moov->iods != NULL)
    if ((ret = write_box(file, box, BOX_IODS, write_iods)) != 0)
      return ret;

  for (i = 0; i < moov->trak_len; i++) {
    box.trak = &moov->trak[i];
    if ((ret = write_box(file, box, BOX_TRAK, write_trak)) != 0)
      return ret;
  }
  return 0;
}

static int
write_mdat(FILE * file, box_t p_box) {
  struct box_moov * moov;
  struct box_stbl * stbl;
  struct box_stco * stco;
  struct box_stsz * stsz;
  unsigned int i;
  unsigned int j;
  unsigned int o; /* stco->entry[o], index of chunk */
  unsigned int z; /* stsz->entry[z], index of sample */
  unsigned int trak_count;
  unsigned int sample_size;
  unsigned int sample_capa;
  unsigned char * sample;
  unsigned int * sample_i; /* sample index for each track */
  FILE * sample_file;
  long pos;
  int ret;

  ret = 0;

  moov = &p_box.top->moov;

  trak_count = 0;

  sample_capa = 1;
  sample_file = p_box.top->mdat.file;

  sample = NULL;
  sample_i = NULL;

  if ((ret = mem_alloc(&sample, sample_capa)) != 0 ||
      (ret = mem_alloc(&sample_i, moov->trak_len * sizeof(sample_i))) != 0)
    goto exit;

  for (i = 0; i < moov->trak_len; i++)
    sample_i[i] = 0;

  /* iterate each chunk */
  for (o = 0;; o++) {

    /* iterate each track */
    for (i = 0; i < moov->trak_len; i++) {

      stbl = &moov->trak[i].mdia.minf.stbl;
      stco = &stbl->stco;
      stsz = &stbl->stsz;

      if (o >= stco->entry_count) {
        trak_count++;
        break;
      }

      if ((ret = get_pos(&pos, file)) != 0)
        goto exit;

      stco->entry[o].chunk_offset = (unsigned int) pos; /* set chunk offset */

      /* iterate each sample */
      for (j = 0; j < stco->entry[o].samples_per_chunk; j++) {

        z = sample_i[i]++;

        sample_size = stsz->entry[z].entry_size;
        while (sample_size > sample_capa) {
          if ((ret = mem_realloc(&sample, sample_capa << 1)) != 0)
            goto exit;
          sample_capa <<= 1;
        }

        /* write sample */
        if ((ret = set_pos(stsz->entry[z].pos, sample_file)) != 0 ||
            (ret = read_ary(sample, sample_size, 1, sample_file)) != 0 ||
            (ret = write_ary(sample, sample_size, 1, file)) != 0)
          goto exit;
      }
    }

    if (trak_count == moov->trak_len)
      break;
  }

  /* iterate each track */
  for (i = 0; i < moov->trak_len; i++) {

    stco = &moov->trak[i].mdia.minf.stbl.stco;

    if ((ret = get_pos(&pos, file)) != 0 ||
        (ret = set_pos(stco->pos, file)) != 0)
      goto exit;

    /* write chunk offset */
    for (j = 0; j < stco->entry_count; j++)
      if ((ret = write_u32(stco->entry[j].chunk_offset, file)) != 0)
        goto exit;

    if ((ret = set_pos(pos, file)) != 0)
      goto exit;
  }
exit:
  mem_free(sample_i);
  mem_free(sample);
  return ret;
}

static int
write_top(struct box_top * top, const char * fname) {
  FILE * file;
  box_t box;
  int ret;

  file = fopen(fname, "wb");
  if (file == NULL) {
    ret = ERR_IO;
    goto exit;
  }

  box.top = top;
  if ((ret = write_box(file, box, BOX_FTYP, write_ftyp)) != 0 ||
      (ret = write_box(file, box, BOX_MOOV, write_moov)) != 0 ||
      (ret = write_box(file, box, BOX_MDAT, write_mdat)) != 0)
    goto close;

close:
  fclose(file);
exit:
  return ret;
}

static int
extract_audio(struct box_top * top) {
  struct box_moov * moov;
  struct box_trak * trak;
  unsigned int i;
  unsigned int j;
  int ret;

  ret = 0;

  moov = &top->moov;

  for (i = 0; i < moov->trak_len; i++) {

    if (moov->trak[i].mdia.hdlr.type == BOX_SOUN) {

      if ((ret = mem_alloc(&trak, sizeof(* trak))) != 0)
        return ret;

      * trak = moov->trak[i];

      for (j = 0; j < moov->trak_len; j++)
        if (j != i)
          free_trak(&moov->trak[j]);
      mem_free(moov->trak);

      trak->tkhd.track_id = 1;
      moov->trak_len = 1;
      moov->trak = trak;
      moov->mvhd.next_track_id = 2;
      return ret;
    }
  }
  return ERR_NO_SOUN;
}

int
main(int argc, char ** argv) {
  const char * input;
  const char * output;
  FILE * file;
  struct box_top top;
  int ret;

  ret = 0;

  if (argc == 0)
    goto exit;

  if (argc == 1 || argc >= 4) {
    printf("%s <INPUT> <OUTPUT>\n", argv[0]);
    goto exit;
  }

  if (argc >= 2)
    input = argv[1];

  if (argc >= 3)
    output = argv[2];
  else
    output = NULL;

  if ((ret = open_file(&file, input)) != 0)
    goto exit;

  if ((ret = read_top(file, &top)) != 0)
    goto close;

  if (output != NULL)
    if ((ret = extract_audio(&top)) != 0 ||
        (ret = write_top(&top, output)) != 0)
      goto free;

free:
  free_top(&top);
close:
  close_file(file);
exit:
  if (ret)
    printf("Error: %s\n", err_to_str(ret));
  return ret;
}
