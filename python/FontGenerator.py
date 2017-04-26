import os
import subprocess

fonts_to_render = [
    {'set': 'ui',
     'subset': [{'fontSize': 10, 'font': 'Tahoma'},
                {'fontSize': 12, 'font': 'Tahoma'},
                {'fontSize': 14, 'font': 'Tahoma'},
                {'fontSize': 16, 'font': 'Tahoma'},
                {'fontSize': 20, 'font': 'Tahoma'}]},
    {'set': 'hud',
     'subset': [{'fontSize': 16, 'font': 'Tahoma'},
                {'fontSize': 24, 'font': 'Tahoma'},
                {'fontSize': 36, 'font': 'Tahoma'},
                {'fontSize': 48, 'font': 'Tahoma'}]}
]

def generate_config_file(args):
    print(args)
    content = """
# AngelCode Bitmap Font Generator configuration file
fileVersion=1

# font settings
fontName={font}
fontFile={fontFile}
charSet=0
fontSize=-{fontSize}
aa=1
scaleH=100
useSmoothing={useSmoothing}
isBold=0
isItalic=0
useUnicode=1
disableBoxChars=1
outputInvalidCharGlyph=1
dontIncludeKerningPairs=0
useHinting=1
renderFromOutline=0
useClearType=1

# character alignment
paddingDown=0
paddingUp=0
paddingRight=0
paddingLeft=0
spacingHoriz=1
spacingVert=1
useFixedHeight=0
forceZero=0

# output file
outWidth=256
outHeight=256
outBitDepth=8
fontDescFormat=0
fourChnlPacked=0
textureFormat=png
textureCompression=0
alphaChnl=0
redChnl=4
greenChnl=4
blueChnl=4
invA=0
invR=0
invG=0
invB=0

# outline
outlineThickness=0

# selected chars
chars=32-126

# imported icon images
""".format(**args)

    return content

def render(filename):
    subprocess.call("bmfont64.exe -c config.bmfc -o "+filename+".fnt", shell = True)

def render_subset(configFile, filename, subset):
    size = subset["fontSize"]
    subset["fontFile"] = ""
    subset["useSmoothing"] = 0 if size<19 else 1
    content = generate_config_file(subset)
    file = open(configFile, 'w')
    file.write(content)
    file.close()
    render(filename+"_"+str(size))

def main():
    for set in fonts_to_render:
        filename = set['set']
        for subset in set['subset']:
            render_subset("config.bmfc", filename, subset)

if __name__ == "__main__":
    main()