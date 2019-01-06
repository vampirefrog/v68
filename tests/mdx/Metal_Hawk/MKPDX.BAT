
rem "MHAWK.PDX" ÇçÏê¨ÇµÇ‹Ç∑ÅBÇµÇŒÇÁÇ≠Ç®ë“Çøâ∫Ç≥Ç¢ÅB
echo off

adpcmxf sd.pcm sd2.pcm -m66
adpcmxf sd.pcm sd2.pcm -t0a
adpcmxf tm.pcm th.pcm -f5
adpcmxf tm.pcm tl.pcm -f-5

adpcmxf c3.pcm c#3.pcm -f1
adpcmxf c3.pcm d3.pcm -f2
adpcmxf c3.pcm d#3.pcm -f3
adpcmxf c3.pcm e3.pcm -f4
adpcmxf c3.pcm f3.pcm -f5
adpcmxf c3.pcm f#3.pcm -f6
adpcmxf c3.pcm g3.pcm -f7
adpcmxf c3.pcm g#3.pcm -f8
adpcmxf c3.pcm a3.pcm -f9
adpcmxf c3.pcm a#3.pcm -f10
adpcmxf c3.pcm b3.pcm -f11

tpdxm mhawk
