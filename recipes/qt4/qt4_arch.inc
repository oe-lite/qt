inherit siteinfo

#ARM_INSTRUCTION_SET = "arm"

def qt_arch(d):
    import bb, re
    arch = bb.data.getVar('TARGET_ARCH', d, 1)
    if re.match("^i.86-", arch):
        arch = "i386"
    elif re.match("^arm.*", arch):
        arch = "arm"
    elif re.match("x86_64-", arch):
        arch = "x86"
    elif re.match("mips.*", arch):
        arch = "mips"
    return arch

def qt_endian(d):
    import bb
    if bb.data.getVar('TARGET_ENDIAN', d, True) == "l":
        return "-little-endian"
    elif bb.data.getVar('TARGET_ENDIAN', d, True) == "b":
        return "-big-endian"
    else:
        assert False
