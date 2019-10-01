const fs = require('fs');

var alphabet = [ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" ]

// https://onehouronelife.com/forums/viewtopic.php?id=6315
var forumList = [ "E", "I", "A", "R", "N", "O", "T", "L", "D", "S", "U", "C", "M", "P", "Y", "B", "G", "F", "V", "NG", "ST", "H", "W", "OU", "EA", "SS", "BL", "NT", "SH", "K", "TR", "CH", "IO", "PR", "Z", "TH", "CK", "OO", "X", "EE", "IE", "AI", "ND", "GR", "J", "PL", "CR", "BR", "LL", "CL", "Q", "FL", "SP", "NC", "PH", "AU", "RD", "DR", "AY", "RT", "FR", "UI", "GL", "SM", "UE", "OA", "SC", "OI", "RS", "IA", "RN", "STR", "SL", "UA", "CT", "GHT", "NK", "LD", "NS", "RK", "MP", "WH", "TCH", "EI", "SK", "RM", "DG", "SW", "EY", "LT", "WN", "SN", "SCR", "TL", "DL", "FF", "GH", "FT", "OY", "WR", "RG", "GN", "EU", "NCH", "LK", "THR", "IOU", "PT", "OE", "PS", "KN", "TT", "SQ", "AE", "RV", "RTH", "CS", "MB", "RCH", "LV", "RL", "TW", "TS", "YA", "RH", "SPR", "RC", "WL", "LM", "WS", "RB", "LS", "CHR", "SCH", "RR", "LF", "EYE", "SHR", "RP", "MS", "EAU", "PP", "NGS", "EO", "NN", "SPL", "ZL", "NTH", "YO", "YE", "SPH", "LP", "KL", "DS", "RST", "MPT", "UO", "LTH", "GG", "WK", "MN", "ZZ", "WD", "NX", "NDS", "TZ", "CKS", "RF", "EOU", "PHR", "RDS", "KR", "YEA", "NCT", "BT", "YOU", "UY", "UEA", "RTS", "LG", "DD", "CHL", "NTS", "MM", "DW", "DH", "AYE", "RSH", "RKS", "NGTH", "KH", "GS", "YU", "UEE", "NKS", "LLS", "LCH", "IEU", "AA", "WZ", "HN", "BS", "UAI", "STH", "RLD", "PHL", "MPS", "IU", "XT", "RTZ", "NTZ", "GHTS", "CZ", "YI", "UOI", "WNS", "TTS", "RNT", "MPH", "LMS", "LB", "DTH", "WTH", "VR", "THW", "RRH", "RQ", "RNS", "RMS", "RGH", "HM", "GM", "FFS", "BB", "UOY", "THM", "SV", "SCL", "SCHL", "RPS", "PN" ];

var endingConsonantClusters = [ "nched", "ngued", "ngues", "rched", "rscht", "tched", "bbed", "ched", "cked", "dged", "ffed", "gged", "ghth", "ghts", "gned", "gued", "gues", "lfth", "lged", "lked", "lled", "lmed", "lque", "lsed", "lved", "lves", "mbed", "mmed", "mned", "mped", "mpse", "nced", "nche", "ncts", "ndth", "nged", "ngst", "ngth", "ngue", "nked", "nned", "nsed", "nstr", "nthe", "ntze", "pped", "ppes", "psed", "qued", "rbed", "rced", "rged", "rgne", "rgue", "rked", "rled", "rles", "rmed", "rmth", "rned", "rpse", "rque", "rred", "rsch", "rsed", "rsts", "rved", "rves", "sced", "shed", "sked", "sque", "ssed", "thed", "thes", "tsch", "ttes", "wned", "wnst", "bed", "bel", "bes", "ble", "ced", "che", "cht", "cks", "cts", "dds", "des", "dge", "dst", "dth", "fed", "ffe", "ffs", "fth", "ged", "ght", "gne", "gue", "hns", "ked", "kes", "lch", "lct", "lds", "ldt", "led", "les", "lft", "lge", "lle", "lls", "lms", "lph", "lpt", "lse", "lsh", "lte", "lth", "lts", "ltz", "lve", "mbs", "med", "mes", "mme", "mph", "mps", "mpt", "nce", "nch", "nct", "nde", "nds", "ned", "nes", "nge", "ngs", "nkh", "nks", "nne", "nns", "nse", "nst", "nte", "nth", "nts", "ntz", "nze", "ped", "pes", "phs", "ppe", "pse", "pth", "que", "rbs", "rce", "rch", "rck", "rcs", "rct", "rde", "rds", "red", "res", "rge", "rgh", "rks", "rld", "rme", "rms", "rne", "rns", "rnt", "rps", "rpt", "rre", "rrh", "rse", "rsh", "rst", "rte", "rth", "rts", "rtz", "rve", "rze", "sce", "sed", "sle", "sne", "sse", "ste", "str", "sts", "tch", "ted", "tes", "the", "thm", "tte", "ttn", "tts", "ved", "ves", "wed", "wls", "wns", "wse", "wth", "xed", "xth", "zed", "bb", "be", "bs", "bt", "ce", "ch", "ck", "cs", "ct", "dd", "de", "dh", "dl", "ds", "dz", "fe", "ff", "fs", "ft", "ge", "gg", "gh", "gm", "gn", "gs", "hm", "hn", "hr", "hs", "ht", "ke", "ks", "lb", "lc", "ld", "le", "lf", "lk", "ll", "lm", "ln", "lp", "ls", "lt", "mb", "me", "mn", "mp", "ms", "mt", "nc", "nd", "ne", "ng", "nk", "nn", "ns", "nt", "nx", "pe", "ph", "pp", "ps", "pt", "rb", "rc", "rd", "re", "rf", "rg", "rk", "rl", "rm", "rn", "rp", "rr", "rs", "rt", "sc", "se", "sh", "sk", "sm", "sp", "ss", "st", "sz", "te", "th", "ts", "tt", "tz", "ve", "vt", "wd", "wk", "wl", "wn", "wp", "ws", "wt", "xe", "xt", "ze", "zz", "b", "c", "d", "f", "g", "h", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "z" ];

var endingConsonantClustersFreq = [ 1,  2,  1,  1,  1,  4,  3,  12,  17,  12,  4,  15,  1,  6,  9,  3,  1,  1,  3,  1,  67,  1,  1,  2,  12,  7,  3,  9,  1,  3,  1,  21,  2,  1,  1,  22,  2,  2,  3,  3,  17,  6,  1,  1,  1,  28,  1,  3,  1,  5,  6,  15,  1,  1,  7,  3,  1,  12,  1,  12,  1,  4,  30,  1,  15,  1,  8,  3,  5,  15,  2,  9,  21,  9,  2,  2,  1,  3,  1,  10,  1,  2,  1,  54,  16,  1,  16,  1,  1,  8,  45,  2,  4,  3,  2,  4,  1,  29,  126,  4,  25,  2,  23,  5,  7,  1,  1,  1,  86,  10,  2,  5,  36,  9,  2,  1,  1,  6,  2,  1,  6,  2,  1,  19,  1,  38,  3,  2,  5,  4,  10,  218,  51,  11,  4,  16,  86,  12,  39,  33,  1,  7,  2,  1,  23,  1,  5,  26,  9,  2,  1,  14,  10,  1,  3,  9,  1,  21,  1,  15,  26,  1,  1,  1,  2,  12,  145,  6,  37,  3,  7,  2,  1,  4,  9,  4,  5,  1,  1,  3,  2,  53,  3,  16,  3,  22,  8,  5,  13,  1,  10,  105,  2,  1,  11,  19,  1,  1,  64,  2,  12,  23,  3,  38,  1,  2,  52,  18,  10,  1,  2,  3,  4,  12,  1,  68,  1,  45,  8,  3,  166,  86,  307,  70,  150,  3,  275,  1,  1,  23,  1,  24,  73,  2,  60,  127,  1,  36,  4,  26,  9,  3,  5,  1,  3,  1,  180,  3,  3,  1,  119,  211,  23,  42,  248,  17,  3,  8,  23,  78,  32,  160,  9,  58,  35,  2,  4,  340,  472,  2917,  91,  4,  86,  953,  12,  92,  32,  3,  30,  60,  21,  1,  276,  386,  8,  17,  112,  29,  91,  139,  14,  6,  118,  167,  2,  336,  345,  25,  241,  17,  1119,  1154,  2,  800,  129,  51,  14,  6,  450,  2,  5,  9,  28,  90,  1,  16,  1,  1,  5,  155,  7,  99,  915,  2093,  65,  149,  40,  136,  1706,  625,  2726,  357,  2,  4592,  1504,  983,  11,  261,  138,  6 ]; 

var middleConsonantClusters = [ "ckscr", "ckstr", "ckthr", "ffspr", "ghtcl", "ghtdr", "ghtsh", "ghtsp", "ghtst", "lkscr", "llspr", "llstr", "lmstr", "mbscr", "mpstr", "ndspr", "ndthr", "ngspr", "ngstr", "ngthw", "rkscr", "rstwh", "rthpl", "rthst", "rthwh", "rtstr", "rtthr", "stscr", "ststr", "tchbl", "tchcr", "wnstr", "bscr", "bstr", "chsh", "chst", "ckbr", "ckcl", "ckcr", "ckdr", "ckgr", "ckkn", "ckpr", "cksc", "cksh", "cksk", "cksl", "cksm", "cksp", "ckst", "cksw", "ckth", "cktr", "ckwh", "dspr", "dstr", "dthw", "ffpr", "ffsh", "ffst", "ftsm", "ggpl", "ggsh", "ghbr", "ghsh", "ghtb", "ghtc", "ghtf", "ghtg", "ghth", "ghtl", "ghtm", "ghtn", "ghtr", "ghts", "ghtt", "ghtw", "kthr", "lchr", "ldbr", "ldsm", "ldsp", "ldst", "lfsb", "lfsk", "llbl", "llfl", "llfr", "llgr", "llpl", "llsp", "llst", "llwr", "lmsh", "lmsm", "lptr", "lstr", "lthf", "ltst", "mbpr", "mbsh", "mbsk", "mbst", "mpbl", "mpcr", "mpgr", "mphl", "mpsh", "mpst", "mptb", "mptl", "mptn", "mptr", "mscr", "mstr", "nchb", "nchl", "nchm", "nchp", "nchr", "ncht", "nchw", "nctl", "ndbl", "ndbr", "ndch", "ndcl", "ndcr", "ndfl", "ndgr", "ndpr", "ndsc", "ndsh", "ndsl", "ndsm", "ndsp", "ndst", "ndsw", "ndtr", "ndwr", "ngpl", "ngsh", "ngsp", "ngst", "ngth", "ngwr", "nkbl", "nksg", "nksh", "nkst", "nsch", "nscr", "nsgr", "nshr", "nspl", "nspr", "nssh", "nstr", "ntbr", "ntcl", "nthl", "nthr", "ntsm", "ntsv", "phth", "pstr", "rbst", "rchb", "rchd", "rchf", "rchg", "rchl", "rchm", "rchn", "rchw", "rdbr", "rdpl", "rdsh", "rdsm", "rdst", "rksh", "rksk", "rksm", "rksp", "rksw", "rldl", "rldw", "rlfr", "rmch", "rmfr", "rmst", "rncr", "rnfl", "rnsh", "rnst", "rpsm", "rsch", "rshl", "rshm", "rshn", "rspl", "rspr", "rstb", "rsth", "rstl", "rstr", "rtbr", "rthb", "rthd", "rthf", "rthl", "rthm", "rthq", "rthr", "rths", "rthw", "rtsc", "rtsh", "rtsm", "rtst", "rtsw", "rtwh", "schl", "schm", "schn", "schw", "shcl", "shst", "shtr", "shwh", "ssbr", "sscr", "stgr", "sthm", "stpl", "stpr", "stwr", "tchb", "tchc", "tchd", "tchf", "tchh", "tchk", "tchl", "tchm", "tchp", "tcht", "tchw", "thbl", "thbr", "thdr", "thpr", "thst", "thtr", "tscr", "tspr", "tstr", "tthr", "ttsb", "ttsd", "ttsf", "tzkr", "wnbr", "wngr", "wnsf", "wnsh", "wnsm", "wnsp", "wnst", "wnsw", "wntr", "wspr", "wsst", "wstr", "bbl", "bbr", "bch", "bcl", "bcr", "bdr", "bfl", "bgl", "bgr", "bph", "bpl", "bsc", "bsl", "bsp", "bst", "btf", "btl", "btr", "bwh", "cch", "ccl", "ccr", "chd", "chf", "chh", "chl", "chm", "chn", "chr", "chs", "cht", "ckb", "ckd", "ckf", "ckg", "ckh", "ckj", "ckl", "ckm", "ckn", "ckp", "ckr", "cks", "ckt", "ckw", "cst", "ctf", "ctl", "ctm", "ctn", "ctr", "dbl", "dbr", "dch", "dcl", "dcr", "ddb", "ddf", "ddh", "ddl", "ddm", "ddn", "ddr", "dfl", "dfr", "dgl", "dgm", "dhp", "dph", "dsh", "dsk", "dsl", "dsm", "dsp", "dst", "dsw", "ffb", "ffh", "ffl", "ffn", "ffr", "ffs", "fgh", "fst", "ftb", "ftl", "ftn", "ftr", "ftw", "ggb", "ggh", "ggl", "ggn", "ggr", "ghb", "ghc", "ghd", "ghf", "ghg", "ghh", "ghl", "ghn", "ghp", "ghr", "ght", "ghw", "gnb", "gnl", "gnm", "gnp", "gnt", "gsh", "gsk", "gst", "gtr", "gwh", "hlr", "hmm", "hnn", "hns", "kch", "kfr", "kpl", "ksh", "kst", "lbl", "lbr", "lch", "lcl", "lcr", "ldb", "ldc", "ldf", "ldh", "ldl", "ldn", "ldp", "ldr", "ldw", "lfb", "lfh", "lfl", "lfp", "lfr", "lfs", "lft", "lfw", "lgr", "lkb", "lkh", "lkl", "lkm", "lks", "lkw", "llb", "llc", "lld", "llf", "llg", "llh", "llm", "lln", "llp", "llr", "lls", "llt", "llw", "lmd", "lmg", "lml", "lmn", "lms", "lpf", "lph", "lpl", "lpm", "lpr", "lpt", "lpw", "lsc", "lsh", "lsk", "lsm", "lsn", "lsp", "lst", "ltb", "ltc", "ltf", "lth", "ltl", "ltm", "ltn", "ltp", "ltr", "ltw", "ltz", "mbb", "mbd", "mbf", "mbk", "mbl", "mbm", "mbn", "mbr", "mbs", "mbt", "mbw", "mcr", "mdr", "mfl", "mkh", "mnb", "mnd", "mnl", "mnn", "mpb", "mpf", "mph", "mpk", "mpl", "mpm", "mpn", "mpp", "mpr", "mps", "mpt", "msd", "msh", "msk", "msm", "msp", "mst", "nbl", "nbr", "nch", "ncl", "ncm", "ncr", "nct", "ndb", "ndc", "ndd", "ndf", "ndg", "ndh", "ndj", "ndk", "ndl", "ndm", "ndn", "ndp", "ndr", "nds", "ndw", "nfl", "nfr", "ngb", "ngd", "ngf", "ngh", "ngk", "ngl", "ngm", "ngn", "ngp", "ngr", "ngs", "ngt", "ngw", "nkb", "nkc", "nkf", "nkg", "nkh", "nkl", "nkm", "nkn", "nkr", "nkt", "nkw", "nnk", "nns", "npl", "npr", "nrh", "nsb", "nsc", "nsd", "nsf", "nsh", "nsk", "nsl", "nsm", "nsn", "nsp", "nst", "nsv", "nsw", "ntd", "ntf", "ntg", "nth", "ntl", "ntm", "ntn", "ntp", "ntr", "nts", "ntw", "ntz", "nwh", "nwr", "pbr", "pch", "pdr", "pfl", "pfr", "pgr", "phl", "phr", "pkn", "pph", "ppl", "ppr", "psc", "psh", "psk", "psq", "pst", "psw", "ptc", "ptl", "ptn", "ptr", "pwr", "rbl", "rbr", "rch", "rcl", "rcr", "rct", "rdb", "rdc", "rdf", "rdh", "rdl", "rdm", "rdn", "rdp", "rdr", "rds", "rdt", "rdv", "rdw", "rfb", "rfd", "rfl", "rfr", "rgh", "rgl", "rgn", "rgr", "rkb", "rkd", "rkh", "rkk", "rkl", "rkm", "rkn", "rkr", "rks", "rkt", "rkw", "rld", "rlh", "rlp", "rlw", "rmb", "rmf", "rmh", "rml", "rmn", "rmp", "rmr", "rmw", "rnb", "rnc", "rnd", "rnf", "rnk", "rnl", "rnm", "rnn", "rnp", "rns", "rnt", "rph", "rpl", "rpm", "rpn", "rpr", "rps", "rpt", "rrh", "rsb", "rsc", "rsd", "rsf", "rsh", "rsk", "rsl", "rsm", "rsn", "rsp", "rsq", "rst", "rsw", "rtb", "rtc", "rtf", "rtg", "rth", "rtl", "rtm", "rtn", "rtr", "rts", "rtw", "rtz", "rwh", "rwr", "sch", "scl", "scr", "sdr", "sgr", "shb", "shc", "shd", "shf", "shh", "shk", "shl", "shm", "shn", "shp", "shr", "sht", "shv", "shw", "skl", "skm", "skn", "skr", "spb", "sph", "spl", "spn", "spr", "ssb", "ssc", "ssf", "ssh", "ssk", "ssl", "ssm", "ssn", "ssp", "ssr", "sst", "ssw", "stb", "stc", "std", "stf", "sth", "stl", "stm", "stn", "stp", "str", "sts", "stw", "tbl", "tbr", "tch", "tcl", "tcr", "tfl", "tgr", "thb", "thf", "thh", "thl", "thm", "thn", "thp", "thr", "ths", "tht", "thw", "tpl", "tpr", "tsb", "tsc", "tsh", "tsk", "tsm", "tsp", "tst", "tsw", "tth", "ttl", "ttr", "tts", "tzp", "tzv", "wbr", "wcl", "wdl", "wdn", "wdr", "wfl", "wkn", "wkw", "wnb", "wnc", "wnf", "wnh", "wnp", "wnr", "wnt", "wnw", "wpl", "wsb", "wsc", "wsd", "wsg", "wsh", "wsl", "wsm", "wsp", "wsr", "wst", "wsw", "wth", "xbl", "xch", "xcl", "xcr", "xgl", "xpl", "xpr", "xsw", "xtb", "xtr", "zzl", "zzm", "bb", "bc", "bd", "bf", "bg", "bh", "bj", "bk", "bl", "bm", "bn", "bp", "br", "bs", "bt", "bv", "bw", "cc", "cd", "ch", "ck", "cl", "cm", "cn", "cq", "cr", "cs", "ct", "cz", "db", "dc", "dd", "df", "dg", "dh", "dj", "dk", "dl", "dm", "dn", "dp", "dq", "dr", "ds", "dt", "dv", "dw", "dz", "fb", "fc", "fd", "ff", "fh", "fj", "fl", "fn", "fr", "ft", "gb", "gc", "gd", "gf", "gg", "gh", "gj", "gl", "gm", "gn", "gp", "gr", "gs", "gt", "gw", "gz", "hd", "hl", "hr", "jj", "kb", "kc", "kd", "kf", "kh", "kj", "kk", "kl", "km", "kn", "kp", "kr", "ks", "kt", "kv", "kw", "lb", "lc", "ld", "lf", "lg", "lh", "lk", "ll", "lm", "ln", "lp", "lq", "lr", "ls", "lt", "lv", "lw", "mb", "mc", "md", "mf", "mh", "mj", "ml", "mm", "mn", "mp", "mq", "mr", "ms", "mt", "mv", "mw", "nb", "nc", "nd", "nf", "ng", "nh", "nj", "nk", "nl", "nm", "nn", "np", "nq", "nr", "ns", "nt", "nv", "nw", "nx", "nz", "pb", "pc", "pd", "pf", "pg", "ph", "pj", "pk", "pl", "pm", "pn", "pp", "pr", "ps", "pt", "pw", "rb", "rc", "rd", "rf", "rg", "rh", "rj", "rk", "rl", "rm", "rn", "rp", "rq", "rr", "rs", "rt", "rv", "rw", "rz", "sb", "sc", "sd", "sf", "sg", "sh", "sj", "sk", "sl", "sm", "sn", "sp", "sq", "sr", "ss", "st", "sv", "sw", "tb", "tc", "td", "tf", "tg", "th", "tk", "tl", "tm", "tn", "tp", "tr", "ts", "tt", "tv", "tw", "tz", "vr", "vv", "vz", "wb", "wc", "wd", "wf", "wg", "wh", "wk", "wl", "wm", "wn", "wp", "wr", "ws", "wt", "ww", "wz", "xb", "xc", "xf", "xh", "xl", "xn", "xp", "xq", "xt", "xv", "xw", "zd", "zl", "zm", "zt", "zv", "zw", "zz", "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "z" ];

var middleConsonantClustersFreq = [ 1,  5,  1,  1,  1,  1,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  1,  2,  1,  1,  1,  1,  1,  2,  1,  2,  4,  13,  1,  1,  1,  1,  1,  1,  1,  2,  1,  1,  5,  2,  2,  3,  3,  7,  1,  1,  1,  1,  2,  3,  1,  1,  2,  1,  3,  1,  1,  4,  1,  1,  1,  9,  1,  4,  17,  2,  5,  2,  1,  1,  5,  1,  2,  1,  2,  1,  2,  1,  1,  2,  3,  1,  1,  1,  1,  3,  1,  2,  1,  1,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  1,  1,  1,  2,  2,  2,  2,  2,  4,  2,  6,  1,  1,  3,  2,  1,  2,  1,  1,  1,  1,  2,  3,  3,  1,  3,  1,  7,  1,  1,  2,  1,  4,  1,  10,  7,  1,  1,  1,  2,  1,  1,  15,  2,  4,  1,  2,  2,  40,  1,  1,  3,  15,  1,  1,  6,  2,  1,  1,  3,  1,  2,  2,  2,  1,  3,  1,  2,  5,  3,  1,  2,  1,  1,  2,  2,  4,  1,  1,  1,  1,  1,  1,  1,  1,  5,  1,  1,  2,  1,  1,  1,  2,  1,  1,  2,  4,  2,  2,  1,  2,  7,  1,  1,  6,  1,  9,  1,  2,  2,  1,  1,  1,  3,  2,  2,  1,  2,  1,  1,  1,  2,  1,  1,  7,  1,  1,  2,  7,  1,  1,  3,  2,  1,  3,  7,  2,  1,  4,  1,  1,  4,  1,  3,  1,  1,  1,  4,  1,  1,  1,  1,  1,  1,  2,  1,  2,  1,  2,  4,  1,  2,  2,  1,  2,  51,  6,  2,  2,  1,  1,  1,  2,  1,  1,  1,  16,  3,  1,  25,  2,  7,  7,  1,  6,  13,  11,  1,  1,  1,  19,  8,  15,  44,  3,  3,  19,  4,  7,  2,  12,  2,  91,  6,  15,  12,  12,  12,  6,  14,  2,  5,  14,  3,  6,  33,  1,  1,  2,  2,  1,  1,  2,  4,  59,  1,  1,  6,  1,  1,  1,  5,  3,  2,  6,  2,  1,  3,  1,  13,  1,  2,  1,  54,  2,  11,  3,  2,  2,  1,  7,  4,  1,  4,  1,  1,  41,  1,  11,  7,  1,  1,  2,  1,  3,  9,  6,  1,  1,  84,  2,  1,  2,  2,  1,  1,  2,  1,  5,  1,  1,  1,  1,  2,  1,  1,  1,  1,  2,  2,  1,  2,  9,  2,  1,  3,  1,  8,  2,  10,  5,  1,  10,  1,  1,  2,  5,  2,  3,  1,  1,  2,  4,  1,  1,  2,  2,  4,  4,  15,  2,  10,  9,  1,  9,  4,  8,  6,  3,  2,  2,  4,  1,  1,  1,  1,  1,  3,  14,  5,  2,  3,  4,  1,  1,  8,  2,  1,  2,  2,  13,  1,  1,  1,  18,  2,  1,  1,  1,  35,  3,  3,  1,  1,  1,  1,  72,  2,  3,  42,  1,  1,  1,  1,  2,  2,  1,  1,  1,  1,  1,  2,  1,  25,  3,  106,  1,  2,  1,  60,  7,  51,  1,  6,  1,  1,  1,  10,  6,  11,  88,  60,  1,  34,  44,  13,  4,  1,  8,  1,  8,  1,  1,  79,  16,  11,  10,  58,  10,  18,  37,  22,  8,  6,  9,  17,  2,  227,  4,  4,  1,  59,  3,  10,  2,  1,  1,  12,  1,  5,  29,  2,  12,  3,  2,  1,  1,  1,  22,  14,  2,  1,  22,  3,  18,  33,  6,  16,  22,  7,  41,  92,  2,  12,  2,  4,  3,  43,  128,  6,  8,  1,  176,  2,  6,  4,  3,  9,  3,  1,  2,  1,  1,  3,  6,  19,  2,  2,  62,  42,  2,  7,  2,  1,  21,  1,  1,  5,  5,  2,  2,  16,  8,  82,  14,  5,  7,  7,  1,  2,  6,  30,  2,  5,  1,  14,  2,  3,  1,  3,  1,  1,  12,  3,  2,  13,  2,  13,  2,  2,  4,  1,  10,  2,  3,  4,  3,  1,  5,  1,  1,  1,  1,  1,  4,  4,  12,  1,  1,  1,  1,  4,  2,  1,  4,  1,  7,  8,  1,  2,  1,  1,  24,  17,  1,  1,  20,  4,  5,  11,  1,  3,  1,  1,  43,  3,  6,  4,  3,  18,  1,  28,  3,  2,  4,  9,  6,  61,  38,  8,  13,  14,  5,  3,  1,  3,  3,  53,  12,  134,  2,  6,  9,  2,  1,  6,  1,  1,  56,  17,  12,  4,  42,  4,  1,  7,  1,  2,  1,  1,  1,  47,  44,  1,  60,  8,  1,  6,  3,  1,  41,  13,  5,  8,  4,  5,  11,  12,  2,  4,  15,  20,  76,  14,  10,  6,  305,  1,  9,  2,  4,  95,  2,  4,  2,  5,  3,  11,  5,  17,  7,  6,  5,  77,  4,  2,  11,  2,  2,  1,  3,  10,  2,  3,  2,  6,  5,  1,  69,  13,  1,  1,  1,  6,  1,  6,  2,  10,  3,  2,  2,  1,  1,  2,  2,  1,  2,  3,  2,  2,  1,  2,  1,  1,  3,  3,  1,  3,  1,  1,  4,  1,  1,  6,  9,  10,  1,  28,  7,  1,  1,  31,  28,  1,  205,  6,  21,  7,  3,  14,  27,  1,  1291,  18,  18,  1,  537,  72,  22,  12,  4,  181,  4,  789,  345,  447,  1,  5,  21,  582,  4,  549,  8,  20,  7,  173,  16,  169,  23,  35,  2,  187,  47,  59,  10,  1,  336,  20,  4,  43,  41,  1,  3,  1,  1,  254,  1,  1,  404,  4,  319,  87,  9,  2,  2,  3,  222,  43,  1,  234,  51,  212,  6,  632,  1,  8,  10,  3,  1,  2,  3,  2,  3,  1,  4,  3,  8,  1,  8,  22,  4,  78,  1,  14,  1,  1,  1,  6,  33,  72,  118,  35,  51,  7,  56,  1411,  70,  28,  36,  1,  16,  66,  206,  120,  16,  272,  8,  3,  20,  2,  1,  36,  364,  61,  445,  1,  8,  19,  3,  5,  5,  81,  917,  922,  250,  454,  109,  53,  152,  151,  63,  444,  54,  45,  101,  502,  1113,  197,  70,  7,  30,  11,  9,  5,  8,  3,  362,  2,  3,  402,  14,  14,  380,  910,  121,  198,  10,  240,  237,  259,  111,  286,  80,  13,  90,  288,  395,  300,  121,  18,  603,  372,  492,  196,  58,  5,  41,  513,  21,  16,  17,  618,  5,  158,  362,  172,  141,  549,  89,  22,  674,  1607,  6,  149,  33,  12,  13,  38,  9,  642,  1,  102,  27,  33,  12,  793,  28,  688,  3,  87,  15,  5,  8,  1,  27,  4,  27,  15,  5,  193,  9,  53,  14,  56,  11,  98,  24,  8,  3,  8,  1,  56,  3,  22,  6,  2,  104,  2,  60,  1,  3,  1,  3,  1,  2,  1,  1,  49,  2995,  5325,  4623,  2200,  2464,  1834,  539,  838,  5717,  4803,  5346,  3396,  423,  7301,  4193,  7226,  2982,  1320,  396,  837 ]; 

var startingConsonantClusters = [ "schl", "schm", "schn", "schw", "chl", "chr", "phl", "phr", "psh", "sch", "scl", "scr", "shr", "sph", "spl", "spr", "str", "thr", "thw", "bh", "bl", "br", "bw", "ch", "cl", "cr", "cz", "dh", "dj", "dr", "dw", "fj", "fl", "fr", "gh", "gl", "gn", "gr", "kh", "kl", "kn", "kr", "kw", "ll", "mb", "mn", "pf", "ph", "pl", "pn", "pr", "ps", "pt", "rh", "rw", "sc", "sf", "sh", "sk", "sl", "sm", "sn", "sp", "sq", "st", "sv", "sw", "th", "tr", "ts", "tw", "wh", "wr", "yt", "zl", "zw", "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "y", "z" ];

var startingConsonantClustersFreq = [ 3,  2,  2,  1,  9,  33,  6,  6,  1,  38,  2,  89,  36,  13,  30,  41,  141,  66,  3,  2,  272,  388,  1,  508,  274,  403,  6,  3,  1,  213,  8,  1,  343,  270,  15,  169,  15,  395,  6,  1,  69,  9,  2,  3,  1,  2,  1,  126,  237,  3,  783,  32,  4,  46,  1,  204,  1,  349,  85,  218,  84,  132,  320,  63,  532,  3,  140,  197,  460,  5,  56,  174,  73,  2,  1,  1,  1985,  2914,  2355,  1345,  893,  1581,  453,  239,  1483,  2231,  851,  2029,  196,  2455,  2009,  1204,  710,  856,  14,  114,  74 ];

var startingVowelClusters = [ "aiia", "ayee", "ayey", "ayoe", "ayoi", "ayou", "eaui", "eyei", "eyey", "iyea", "oeia", "ooey", "ooie", "oyee", "oyeu", "oyou", "ueou", "ueue", "ueui", "uiou", "uoia", "uoya", "aai", "aea", "aeo", "aeu", "aia", "aie", "aii", "aoi", "aou", "aue", "aya", "aye", "ayi", "ayo", "ayu", "eae", "eau", "eea", "eei", "eia", "eio", "eoi", "eou", "eya", "eye", "eyi", "eyo", "iae", "iai", "iao", "iei", "ieu", "ioa", "ioe", "ioi", "iou", "iya", "oau", "oei", "oeu", "oia", "oie", "ooe", "ooi", "oua", "oue", "oui", "oya", "oye", "oyi", "oyo", "uai", "uau", "uay", "uea", "uee", "uei", "ueu", "uey", "uia", "uie", "uiu", "uoi", "uou", "uoy", "uya", "uye", "uyi", "aa", "ae", "ai", "ao", "au", "ay", "ea", "ee", "ei", "eo", "eu", "ey", "ia", "ie", "ii", "io", "iu", "oa", "oe", "oi", "oo", "ou", "oy", "ua", "ue", "ui", "uo", "uu", "uy", "a", "e", "i", "o", "u" ];

var startingVowelClustersFreq = [ 1,  1,  1,  1,  1,  2,  1,  1,  1,  2,  1,  3,  2,  1,  2,  3,  3,  1,  1,  1,  1,  3,  1,  1,  2,  2,  3,  3,  1,  1,  2,  2,  13,  43,  22,  22,  4,  1,  36,  3,  14,  1,  5,  3,  56,  6,  49,  9,  5,  5,  1,  2,  1,  9,  1,  2,  1,  187,  3,  1,  5,  2,  2,  1,  5,  8,  4,  5,  4,  23,  10,  6,  3,  8,  1,  2,  14,  11,  2,  1,  3,  4,  20,  1,  5,  43,  2,  2,  1,  3,  8,  72,  786,  13,  448,  296,  1742,  902,  296,  194,  153,  140,  1181,  2071,  3,  1341,  174,  353,  141,  368,  891,  1408,  68,  449,  392,  418,  46,  6,  8,  20381,  30392,  20256,  13523,  7814 ]; 

var vowelClusters = [ "aiia", "ayee", "ayey", "ayoe", "ayoi", "ayou", "eaui", "eyei", "eyey", "iyea", "oeia", "ooey", "ooie", "oyee", "oyeu", "oyou", "ueou", "ueue", "ueui", "uiou", "uoia", "uoya", "aai", "aea", "aeo", "aeu", "aia", "aie", "aii", "aoi", "aou", "aue", "aya", "aye", "ayi", "ayo", "ayu", "eae", "eau", "eea", "eei", "eia", "eio", "eoi", "eou", "eya", "eye", "eyi", "eyo", "iae", "iai", "iao", "iei", "ieu", "ioa", "ioe", "ioi", "iou", "iya", "oau", "oei", "oeu", "oia", "oie", "ooe", "ooi", "oua", "oue", "oui", "oya", "oye", "oyi", "oyo", "uai", "uau", "uay", "uea", "uee", "uei", "ueu", "uey", "uia", "uie", "uiu", "uoi", "uou", "uoy", "uya", "uye", "uyi", "yea", "yie", "aa", "ae", "ai", "ao", "au", "ay", "ea", "ee", "ei", "eo", "eu", "ey", "ia", "ie", "ii", "io", "iu", "oa", "oe", "oi", "oo", "ou", "oy", "ua", "ue", "ui", "uo", "uu", "uy", "ya", "ye", "yi", "yo", "a", "e", "i", "o", "u", "y" ];

var vowelClustersFreq = [ 1,  1,  1,  1,  1,  2,  1,  1,  1,  2,  1,  3,  2,  1,  2,  3,  3,  1,  1,  1,  1,  3,  1,  1,  2,  2,  3,  3,  1,  1,  2,  2,  13,  43,  22,  22,  4,  1,  36,  3,  14,  1,  5,  3,  56,  6,  49,  9,  5,  5,  1,  2,  1,  9,  1,  2,  1,  187,  3,  1,  5,  2,  2,  1,  5,  8,  4,  5,  4,  23,  10,  6,  3,  8,  1,  2,  14,  11,  2,  1,  3,  4,  20,  1,  5,  43,  2,  2,  1,  3,  2,  1,  8,  72,  786,  13,  448,  296,  1742,  902,  296,  194,  153,  140,  1181,  2071,  3,  1341,  174,  353,  141,  368,  891,  1408,  68,  449,  392,  418,  46,  6,  8,  22,  8,  160,  13,  20381,  30392,  20256,  13523,  7814,  4917 ];

function Cluster(inText, inNumbers) {
	this.text = inText;
	this.numbers = inNumbers;

	this.count = 0;
	this.increaseCount = function () {
		this.count++;
		if (this.count >= this.text.length) {
			this.count = 0;
			return true;
		}
		return false;
	}
	this.get = function() {
		return this.text[this.count];
	}
}

var clusters = {};
clusters.vowelStarting = new Cluster(startingVowelClusters, startingVowelClustersFreq);
clusters.vowel = new Cluster(vowelClusters, vowelClustersFreq);
clusters.consonantStarting = new Cluster(startingConsonantClusters, startingConsonantClustersFreq);
clusters.consonantMiddle = new Cluster(middleConsonantClusters, middleConsonantClustersFreq);
clusters.consonantEnding = new Cluster(endingConsonantClusters, endingConsonantClustersFreq);

function getLength(cluster, minFreq) {
	let count = 0;
	for (var key in cluster.numbers) {
		if (cluster.numbers[key] >= minFreq) count++;
	}
	return count;
}

function sortCluster(cluster, minFreq) {
	let highestNum = 0;
	for (var key in cluster.numbers) {
		if (cluster.numbers[key] > highestNum) highestNum = cluster.numbers[key];
	}
	var outCluster = new Cluster([], []);
	for (var i=highestNum; i >= minFreq; i--) {
		for (var key in cluster.numbers) {
			if (cluster.numbers[key] != i) continue;
			outCluster.numbers.push(cluster.numbers[key]);
			outCluster.text.push(cluster.text[key]);
		}
	}
	return outCluster;
}

function createClusterArray() {
	let minFreq = 20;
	
	for (var key in clusters) {
		console.log(key+".length: "+clusters[key].text.length)
	}
	console.log(" ");
	for (var key in clusters) {
		console.log(key+": "+getLength(clusters[key], minFreq))
	}
	console.log(" ");
	console.log("minFreq: "+minFreq);
	console.log(" ");

	for (var key in clusters) {
		clusters[key] = sortCluster(clusters[key], minFreq);
	}

	let size = clusters.consonantMiddle.text.length*2;
	let size2 = 0;
	let str = "";
	let str2 = "";
	let startType = 1;
	let endType = 1;
	for (var i=0; i < size; i++) {
		str2 = "";
		str2 += '"';

		if (startType === 1) {
			str2 += clusters.vowelStarting.get();
			if (clusters.vowelStarting.increaseCount()) startType = 2;
		} else {
			str2 += clusters.consonantStarting.get();
			if (clusters.consonantStarting.increaseCount()) startType = 1;
		}

		str2 += clusters.consonantMiddle.get();
		clusters.consonantStarting.increaseCount();

		if (endType === 1) {
			str2 += clusters.consonantEnding.get();
			if (clusters.consonantEnding.increaseCount()) endType = 2;
		} else {
			str2 += clusters.vowel.get();
			if (clusters.vowel.increaseCount()) endType = 1;
		}
		
		if (str2.length > size2) size2 = str2.length;
		str += str2.toUpperCase()+'", ';
	}
	str = str.substring(0, str.length-2);
	str += "};";
	str = "static constexpr char languageArray["+size+"]["+size2+"] = { "+str;
	console.log(str);
}

function createStringFromArray(array) {
	let longestWord = 0;
	for (var key in array) {
		if (array[key].length > longestWord) longestWord = array[key].length;
	}
	longestWord++;
	let str = "static constexpr char languageArray["+array.length+"]["+longestWord+"] = { ";
	for (var key in array) {
		str += '"';
		str += array[key];
		str += '", ';
	}
	str = str.substring(0, str.length-2);
	str += "};";
	return str;
}

function main() {
	var array = [];

	function existsInArray(str) {
		for (var key in array) {
			if (array[key] === str.toUpperCase()) return true;
		}
		return false;
	}

	for (var key in alphabet) {
		array.push(alphabet[key].toUpperCase());
	}

	let contents = fs.readFileSync("oholwords", 'utf8');	
	let ohollines = contents.split('\n');
	contents = fs.readFileSync("wordlist", 'utf8');
	let wordlines = contents.split('\n');

	for (var i = 0; i < ohollines.length; i++) {
		if (!existsInArray(ohollines[i])) if (ohollines[i].length > 0) array.push(ohollines[i].toUpperCase());
		if (wordlines[i]) if (!existsInArray(wordlines[i])) if (wordlines[i].length > 0) array.push(wordlines[i].toUpperCase());
	}

	for (var key in forumList) {
		if (!existsInArray(forumList[key])) array.push(forumList[key].toUpperCase());
	}

	console.log(createStringFromArray(array));
}

main()

