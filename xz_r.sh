~/sim/gem5/build/X86/gem5.opt --outdir=/u/home/twayana/stats/xz_r/$1 ~/sim/gem5/configs/example/se.py --cmd=/u/home/twayana/sim/spec-cpu/benchspec/CPU/557.xz_r/build/build_base_initial-m64.0000/xz_r --options="/u/home/twayana/sim/spec-cpu/benchspec/CPU/557.xz_r/run/run_base_refrate_initial-m64.0000/cld.tar.xz 160 19cf30ae51eddcbefda78dd06014b4b96281456e078ca7c13e1c0c9e6aaea8dff3efb4ad6b0456697718cede6bd5454852652806a657bb56e07d61128434b474 59796407 61004416 6" --mem-size=8GB --cpu-type=DerivO3CPU --num-cpu=2 --maxinsts=$2 --caches --l2cache --l1d_size=32kB --l1i_size=32kB --l2_size=512kB --l1i-hwp-type=TaggedPrefetcher --l1d-hwp-type=TaggedPrefetcher --l2-hwp-type=TaggedPrefetcher --prob-hw-counters=1 --prob-hw-counters-ep=0.004 --prob-hw-counters-gamma=0.014
