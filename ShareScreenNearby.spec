# -*- mode: python ; coding: utf-8 -*-

block_cipher = None

a = Analysis(
    ['ShareScreenNearby.py'],
    pathex=['.'],  # Ensure the current directory is in the path
    binaries=[],
    datas=[('src/logo-r.webp', 'src')],  # Include the logo file
    hiddenimports=[],  # Add any hidden imports if needed
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=['tkinter', 'unittest', 'numpy', 'scipy'],  # Exclude unnecessary libraries
    noarchive=False,
)
pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name='ShareScreenNearby',
    debug=False,
    bootloader_ignore_signals=False,
    strip=True,  # Strip the executable to reduce size
    upx=True,  # Use UPX to compress the executable
    upx_exclude=[],  # Optionally exclude specific binaries from UPX compression
    runtime_tmpdir=None,
    console=True,  # Keep the console window
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon='src/logo-r.webp',  # Correct path to the icon file
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=True,  # Strip the binaries in the COLLECT phase
    upx=True,  # Apply UPX compression in the COLLECT phase
    upx_exclude=[],
    name='ShareScreenNearby',
)
