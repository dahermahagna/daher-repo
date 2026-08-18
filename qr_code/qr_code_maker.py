import qrcode
import os
import argparse
import qrcode.constants
from qrcode.image.styledpil import StyledPilImage
from PIL import Image, ImageDraw
from qrcode.image.styles.moduledrawers.pil import StyledPilQRModuleDrawer, RoundedModuleDrawer
from qrcode.image.styles.colormasks import SolidFillColorMask


def parse_args():
    parser = argparse.ArgumentParser(
        description="Generate a styled QR code image.",
    )
    parser.add_argument(
        "-d",
        "--data",
        help="Data/text/URL to encode in the QR code. Uses default from script if omitted.",
    )
    parser.add_argument(
        "-f",
        "--filename",
        help="Output image filename. Uses default from script if omitted.",
    )
    parser.add_argument(
        "-l",
        "--logo",
        help="Logo file to embed in the QR code. Uses default from script if omitted.",
    )
    return parser.parse_args()

def normalize_output_filename(raw_filename, default_extension=".png"):
    name = (raw_filename or "").strip()
    if not name:
        return DEFAULT_FILENAME

    root, ext = os.path.splitext(name)
    if ext:
        return name
    return f"{name}{default_extension}"

def hex_to_rgb(color):
    color = color.strip().lstrip("#")
    if len(color) != 6:
        raise ValueError("Color must be a 6-digit hex value, e.g. #0B1F3A")
    return tuple(int(color[i:i + 2], 16) for i in (0, 2, 4))

class DModuleDrawer(StyledPilQRModuleDrawer):
    """Draw each active QR module as a D-like glyph."""

    def initialize(self, *args, **kwargs):
        super().initialize(*args, **kwargs)
        self.imgDraw = ImageDraw.Draw(self.img._img)

    def drawrect(self, box, is_active: bool):
        if not is_active:
            return

        x0, y0 = box[0]
        x1, y1 = box[1]
        width = max(1, x1 - x0)
        stem_right = x0 + max(1, int(width * 0.42))

        # Draw a right-round body and reinforce left side to look like "D".
        self.imgDraw.ellipse((x0, y0, x1, y1), fill=self.img.paint_color)
        self.imgDraw.rectangle((x0, y0, stem_right, y1), fill=self.img.paint_color)

class MModuleDrawer(StyledPilQRModuleDrawer):
    """Draw each active QR module as a blocky M-like glyph."""

    def initialize(self, *args, **kwargs):
        super().initialize(*args, **kwargs)
        self.imgDraw = ImageDraw.Draw(self.img._img)

    def drawrect(self, box, is_active: bool):
        if not is_active:
            return

        x0, y0 = box[0]
        x1, y1 = box[1]
        w = max(1, x1 - x0)
        h = max(1, y1 - y0)
        stroke = max(1, int(min(w, h) * 0.22))
        mid = (x0 + x1) // 2

        # Left and right vertical stems.
        self.imgDraw.rectangle((x0, y0, x0 + stroke, y1), fill=self.img.paint_color)
        self.imgDraw.rectangle((x1 - stroke, y0, x1, y1), fill=self.img.paint_color)

        # Two diagonals meeting near the center to form an "M".
        self.imgDraw.line((x0 + stroke, y0, mid, y0 + h * 0.55), fill=self.img.paint_color, width=stroke)
        self.imgDraw.line((x1 - stroke, y0, mid, y0 + h * 0.55), fill=self.img.paint_color, width=stroke)

class MedanModuleDrawer(StyledPilQRModuleDrawer):
    """Draw active modules as repeating M/E/D/A/N glyphs."""

    letters = "MEDAN"

    def initialize(self, *args, **kwargs):
        super().initialize(*args, **kwargs)
        self.imgDraw = ImageDraw.Draw(self.img._img)

    def drawrect(self, box, is_active: bool):
        if not is_active:
            return

        x0, y0 = box[0]
        x1, y1 = box[1]
        w = max(1, x1 - x0)
        h = max(1, y1 - y0)
        stroke = max(1, int(min(w, h) * 0.18))
        pad = max(1, int(min(w, h) * 0.10))

        # Pick one of M/E/D/A/N based on module position.
        col = x0 // max(1, self.img.box_size)
        row = y0 // max(1, self.img.box_size)
        ch = self.letters[(row + col) % len(self.letters)]

        left = x0 + pad
        top = y0 + pad
        right = x1 - pad
        bottom = y1 - pad
        midx = (left + right) // 2
        midy = (top + bottom) // 2
        color = self.img.paint_color

        if ch == "M":
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
            self.imgDraw.rectangle((right - stroke, top, right, bottom), fill=color)
            self.imgDraw.line((left + stroke, top, midx, midy), fill=color, width=stroke)
            self.imgDraw.line((right - stroke, top, midx, midy), fill=color, width=stroke)
        elif ch == "E":
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
            self.imgDraw.rectangle((left, top, right, top + stroke), fill=color)
            self.imgDraw.rectangle((left, midy - stroke // 2, right - stroke, midy + stroke // 2), fill=color)
            self.imgDraw.rectangle((left, bottom - stroke, right, bottom), fill=color)
        elif ch == "D":
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
            self.imgDraw.ellipse((left, top, right, bottom), outline=color, width=stroke)
            self.imgDraw.rectangle((left, top, midx, bottom), fill=color)
        elif ch == "A":
            self.imgDraw.line((left, bottom, midx, top), fill=color, width=stroke)
            self.imgDraw.line((midx, top, right, bottom), fill=color, width=stroke)
            self.imgDraw.rectangle((left + stroke, midy - stroke // 2, right - stroke, midy + stroke // 2), fill=color)
        else:  # N
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
            self.imgDraw.rectangle((right - stroke, top, right, bottom), fill=color)
            self.imgDraw.line((left + stroke, top, right - stroke, bottom), fill=color, width=stroke)

class BeTechModuleDrawer(StyledPilQRModuleDrawer):
    """Draw active modules as repeating B/E/T/E/C/H glyphs."""

    letters = "BETECH"

    def initialize(self, *args, **kwargs):
        super().initialize(*args, **kwargs)
        self.imgDraw = ImageDraw.Draw(self.img._img)

    def drawrect(self, box, is_active: bool):
        if not is_active:
            return

        x0, y0 = box[0]
        x1, y1 = box[1]
        w = max(1, x1 - x0)
        h = max(1, y1 - y0)
        stroke = max(1, int(min(w, h) * 0.18))
        pad = max(1, int(min(w, h) * 0.10))

        col = x0 // max(1, self.img.box_size)
        row = y0 // max(1, self.img.box_size)
        ch = self.letters[(row + col) % len(self.letters)]

        left = x0 + pad
        top = y0 + pad
        right = x1 - pad
        bottom = y1 - pad
        midx = (left + right) // 2
        midy = (top + bottom) // 2
        color = self.img.paint_color

        if ch == "B":
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
            self.imgDraw.ellipse((left, top, right, midy + stroke), outline=color, width=stroke)
            self.imgDraw.ellipse((left, midy - stroke, right, bottom), outline=color, width=stroke)
            self.imgDraw.rectangle((left, top, midx, bottom), fill=color)
        elif ch == "E":
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
            self.imgDraw.rectangle((left, top, right, top + stroke), fill=color)
            self.imgDraw.rectangle((left, midy - stroke // 2, right - stroke, midy + stroke // 2), fill=color)
            self.imgDraw.rectangle((left, bottom - stroke, right, bottom), fill=color)
        elif ch == "T":
            self.imgDraw.rectangle((left, top, right, top + stroke), fill=color)
            self.imgDraw.rectangle((midx - stroke // 2, top, midx + stroke // 2, bottom), fill=color)
        elif ch == "C":
            self.imgDraw.ellipse((left, top, right, bottom), outline=color, width=stroke)
            self.imgDraw.rectangle((midx, top, right, bottom), fill=(255, 255, 255))
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
        else:  # H
            self.imgDraw.rectangle((left, top, left + stroke, bottom), fill=color)
            self.imgDraw.rectangle((right - stroke, top, right, bottom), fill=color)
            self.imgDraw.rectangle((left + stroke, midy - stroke // 2, right - stroke, midy + stroke // 2), fill=color)

def _get_pil_image(img_obj):
    if hasattr(img_obj, "get_image"):
        return img_obj.get_image()
    if hasattr(img_obj, "_img"):
        return img_obj._img
    return img_obj

def add_logo_to_finder_squares(img_obj, qr_obj, logo_file, finder_logo_modules=1.8):
    """Overlay logo into the inner finder area with configurable smaller size."""
    pil_img = _get_pil_image(img_obj)
    if not isinstance(pil_img, Image.Image):
        return

    logo = Image.open(logo_file).convert("RGBA")
    # Finder inner area is 3 modules x 3 modules; use a smaller centered logo.
    finder_logo_modules = max(0.8, min(3.0, finder_logo_modules))
    logo_px = max(1, int(qr_obj.box_size * finder_logo_modules))
    logo = logo.resize((logo_px, logo_px), Image.Resampling.LANCZOS)

    module_count = len(qr_obj.modules)
    border = qr_obj.border
    box = qr_obj.box_size

    # Finder starts are at (0,0), (module_count-7,0), (0,module_count-7).
    # Center logo inside each finder's 3x3 inner core.
    inner_size = 3 * box
    delta = max(0, (inner_size - logo_px) // 2)
    top_left = ((border + 2) * box + delta, (border + 2) * box + delta)
    top_right = ((border + module_count - 5) * box + delta, (border + 2) * box + delta)
    bottom_left = ((border + 2) * box + delta, (border + module_count - 5) * box + delta)

    for x, y in (top_left, top_right, bottom_left):
        pil_img.paste(logo, (x, y), logo)

# The data and output file defaults (used if -d or -f are not supplied).
DEFAULT_DATA = "https://www.bznstech.com/business/6c68b320-8292-49a7-aadc-e6ec6d5debf1"
DEFAULT_FILENAME = "saloon_abo_shaker.png"
DEFAULT_LOGO_PATH = "/home/daher/qr_code/logo2.png"

def main():
    args = parse_args()
    data = args.data if args.data else DEFAULT_DATA
    filename = normalize_output_filename(args.filename if args.filename else DEFAULT_FILENAME)
    logo_path = args.logo if args.logo else DEFAULT_LOGO_PATH

    # QR colors (change these hex values to your brand palette).
    FRONT_COLOR_HEX = "#000000"
    BACK_COLOR_HEX = "#FFFFFF"
    qr_color_mask = SolidFillColorMask(
        back_color=hex_to_rgb(BACK_COLOR_HEX),
        front_color=hex_to_rgb(FRONT_COLOR_HEX),
    )

    # Create a QR code with higher error correction to better tolerate center logos.
    qr = qrcode.QRCode(
            error_correction=qrcode.constants.ERROR_CORRECT_H,
        box_size=20,
        border=4,
    )
    qr.add_data(data)
    qr.make(fit=True)

    # Pick your module style: "D", "M", "MEDAN", or "BETECH".
    MODULE_STYLE = "R"
    if MODULE_STYLE.upper() == "M":
        module_drawer = MModuleDrawer()
    elif MODULE_STYLE.upper() == "MEDAN":
        module_drawer = MedanModuleDrawer()
    elif MODULE_STYLE.upper() == "BETECH":
        module_drawer = BeTechModuleDrawer()
    elif MODULE_STYLE.upper() == "R":
        module_drawer = RoundedModuleDrawer()
    else:
        module_drawer = DModuleDrawer()

    # Add logo in the middle (optional)
    if os.path.exists(logo_path):
        img = qr.make_image(
            image_factory=StyledPilImage,
            module_drawer=module_drawer,
            # color_mask=qr_color_mask,
            embeded_image_path=logo_path,
        )
        USE_LOGO_FOR_FINDER_SQUARES = False
        FINDER_LOGO_MODULES = 1.8
        if USE_LOGO_FOR_FINDER_SQUARES:
            add_logo_to_finder_squares(img, qr, logo_path, FINDER_LOGO_MODULES)
    else:
        img = qr.make_image(
            image_factory=StyledPilImage,
            module_drawer=module_drawer,
            # color_mask=qr_color_mask,
        )

    # Upscale after rendering to keep output crisp while avoiding slow high-res color masking.
    OUTPUT_SCALE = 4
    if OUTPUT_SCALE > 1:
        pil_img = _get_pil_image(img)
        img = pil_img.resize(
            (pil_img.width * OUTPUT_SCALE, pil_img.height * OUTPUT_SCALE),
            Image.Resampling.NEAREST,
        )

    # Save the file
    img.save(filename)


if __name__ == "__main__":
    main()