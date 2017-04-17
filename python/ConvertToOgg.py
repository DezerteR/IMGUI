import os, sys, glob, fnmatch
import subprocess

ffmpeg = 'C:/ffmpeg/bin/ffmpeg.exe'

def convertToOgg(file):
	(root, ext) = os.path.splitext(file)
	target = root+ '.ogg'
	print (file)
	print (target)
	print ([ffmpeg,  ' -y -i "' + file + '" -acodec libvorbis -aq 100 "' + target + '"'])

	cmd_list = [ffmpeg,  '-y', '-i']
	cmd_list += [file]
	cmd_list += ['-acodec', 'libvorbis', '-aq', '100']
	cmd_list += [target]

	subprocess.call(cmd_list)
	os.remove(file)


for root, dirnames, filenames in os.walk('../res/audio'):
	for filename in fnmatch.filter(filenames, '*.mp3'):
		convertToOgg(os.path.abspath(os.path.join(root, filename)))


