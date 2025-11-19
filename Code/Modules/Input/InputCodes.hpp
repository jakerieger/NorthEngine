// Author: Jake Rieger
// Created: 11/15/25.
//

#pragma once

#include "Common/Typedefs.hpp"

namespace North::Input {
    /// @brief Key code definitions for keyboard input
    namespace Keys {
        static constexpr u16 Space        = 32;
        static constexpr u16 Apostrophe   = 39; /* ' */
        static constexpr u16 Comma        = 44; /* , */
        static constexpr u16 Minus        = 45; /* - */
        static constexpr u16 Period       = 46; /* . */
        static constexpr u16 Slash        = 47; /* / */
        static constexpr u16 Num0         = 48;
        static constexpr u16 Num1         = 49;
        static constexpr u16 Num2         = 50;
        static constexpr u16 Num3         = 51;
        static constexpr u16 Num4         = 52;
        static constexpr u16 Num5         = 53;
        static constexpr u16 Num6         = 54;
        static constexpr u16 Num7         = 55;
        static constexpr u16 Num8         = 56;
        static constexpr u16 Num9         = 57;
        static constexpr u16 Semicolon    = 59; /* ; */
        static constexpr u16 Equal        = 61; /* = */
        static constexpr u16 A            = 65;
        static constexpr u16 B            = 66;
        static constexpr u16 C            = 67;
        static constexpr u16 D            = 68;
        static constexpr u16 E            = 69;
        static constexpr u16 F            = 70;
        static constexpr u16 G            = 71;
        static constexpr u16 H            = 72;
        static constexpr u16 I            = 73;
        static constexpr u16 J            = 74;
        static constexpr u16 K            = 75;
        static constexpr u16 L            = 76;
        static constexpr u16 M            = 77;
        static constexpr u16 N            = 78;
        static constexpr u16 O            = 79;
        static constexpr u16 P            = 80;
        static constexpr u16 Q            = 81;
        static constexpr u16 R            = 82;
        static constexpr u16 S            = 83;
        static constexpr u16 T            = 84;
        static constexpr u16 U            = 85;
        static constexpr u16 V            = 86;
        static constexpr u16 W            = 87;
        static constexpr u16 X            = 88;
        static constexpr u16 Y            = 89;
        static constexpr u16 Z            = 90;
        static constexpr u16 LeftBracket  = 91;  /* [ */
        static constexpr u16 Backslash    = 92;  /* \ */
        static constexpr u16 RightBracket = 93;  /* ] */
        static constexpr u16 GraveAccent  = 96;  /* ` */
        static constexpr u16 World1       = 161; /* non-US #1 */
        static constexpr u16 World2       = 162; /* non-US #2 */

        static constexpr u16 Escape      = 256;
        static constexpr u16 Enter       = 257;
        static constexpr u16 Tab         = 258;
        static constexpr u16 Backspace   = 259;
        static constexpr u16 Insert      = 260;
        static constexpr u16 Delete      = 261;
        static constexpr u16 Right       = 262;
        static constexpr u16 Left        = 263;
        static constexpr u16 Down        = 264;
        static constexpr u16 Up          = 265;
        static constexpr u16 PageUp      = 266;
        static constexpr u16 PageDown    = 267;
        static constexpr u16 Home        = 268;
        static constexpr u16 End         = 269;
        static constexpr u16 CapsLock    = 280;
        static constexpr u16 ScrollLock  = 281;
        static constexpr u16 NumLock     = 282;
        static constexpr u16 PrintScreen = 283;
        static constexpr u16 Pause       = 284;
        static constexpr u16 F1          = 290;
        static constexpr u16 F2          = 291;
        static constexpr u16 F3          = 292;
        static constexpr u16 F4          = 293;
        static constexpr u16 F5          = 294;
        static constexpr u16 F6          = 295;
        static constexpr u16 F7          = 296;
        static constexpr u16 F8          = 297;
        static constexpr u16 F9          = 298;
        static constexpr u16 F10         = 299;
        static constexpr u16 F11         = 300;
        static constexpr u16 F12         = 301;
        static constexpr u16 F13         = 302;
        static constexpr u16 F14         = 303;
        static constexpr u16 F15         = 304;
        static constexpr u16 F16         = 305;
        static constexpr u16 F17         = 306;
        static constexpr u16 F18         = 307;
        static constexpr u16 F19         = 308;
        static constexpr u16 F20         = 309;
        static constexpr u16 F21         = 310;
        static constexpr u16 F22         = 311;
        static constexpr u16 F23         = 312;
        static constexpr u16 F24         = 313;
        static constexpr u16 F25         = 314;

        static constexpr u16 KP0        = 320;
        static constexpr u16 KP1        = 321;
        static constexpr u16 KP2        = 322;
        static constexpr u16 KP3        = 323;
        static constexpr u16 KP4        = 324;
        static constexpr u16 KP5        = 325;
        static constexpr u16 KP6        = 326;
        static constexpr u16 KP7        = 327;
        static constexpr u16 KP8        = 328;
        static constexpr u16 KP9        = 329;
        static constexpr u16 KPDecimal  = 330;
        static constexpr u16 KPDivide   = 331;
        static constexpr u16 KPMultiply = 332;
        static constexpr u16 KPSubtract = 333;
        static constexpr u16 KPAdd      = 334;
        static constexpr u16 KPEnter    = 335;
        static constexpr u16 KPEqual    = 336;

        static constexpr u16 LeftShift    = 340;
        static constexpr u16 LeftControl  = 341;
        static constexpr u16 LeftAlt      = 342;
        static constexpr u16 LeftSuper    = 343;
        static constexpr u16 RightShift   = 344;
        static constexpr u16 RightControl = 345;
        static constexpr u16 RightAlt     = 346;
        static constexpr u16 RightSuper   = 347;
        static constexpr u16 Menu         = 348;

        static constexpr u16 Last = Menu;
    }  // namespace Keys

    /**
     * @brief Mouse button codes for mouse input
     */
    namespace MouseButtons {
        static constexpr u16 Button1 = 0;
        static constexpr u16 Button2 = 1;
        static constexpr u16 Button3 = 2;
        static constexpr u16 Button4 = 3;
        static constexpr u16 Button5 = 4;
        static constexpr u16 Button6 = 5;
        static constexpr u16 Button7 = 6;
        static constexpr u16 Button8 = 7;
        static constexpr u16 Last    = Button8;
        static constexpr u16 Left    = Button1;
        static constexpr u16 Right   = Button2;
        static constexpr u16 Middle  = Button3;
    }  // namespace MouseButtons
}  // namespace North::Input