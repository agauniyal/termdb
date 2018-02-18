#ifndef RANG_TERMDB_HPP
#define RANG_TERMDB_HPP

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#define OS_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define OS_WIN
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_MAC
#else
#error Unknown Platform
#endif

#include "optional.hpp"
#include "variant.hpp"
#include <algorithm>
#include <bitset>
#include <fstream>
#include <limits>
#include <vector>
#include <array>
#include <string>
#include <stack>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <regex>
#include <system_error>

#if defined(OS_WIN)
#include <windows.h>
#include <VersionHelpers.h>
#endif

namespace tdb {
enum class ParseError { Success, ReadError, BadDatabase, MagicByteError };
}  // namespace tdb

namespace std {
template <>
struct is_error_code_enum<tdb::ParseError> : std::true_type {
};
}  // namespace std

/*
 * [1] http://invisible-island.net/ncurses/man/term.5.html
 * [2] $ man terminfo
 * [3] $ man term
 */

namespace tdb {

#if defined(OS_LINUX) || defined(OS_MAC)
constexpr const auto DPATH = "/usr/share/terminfo/";
#elif defined(OS_WIN)
constexpr const auto DPATH = "";
#endif

// sizes from '/usr/include/term.h'
constexpr const auto numCapBool = 44;
constexpr const auto numCapNum  = 39;
constexpr const auto numCapStr  = 414;


// param types
using param = mpark::variant<long, std::string>;


// error_code
namespace detail {
    class ParseError_category : public std::error_category {
    public:
        virtual const char *name() const noexcept override final
        {
            return "DatabaseParseError";
        }
        virtual std::string message(int c) const override final
        {
            switch (static_cast<ParseError>(c)) {
                case ParseError::ReadError:
                    return "Error reading database file";
                case ParseError::BadDatabase:
                    return "Invalid database, might be corrupted";
                case ParseError::MagicByteError: return "Magic byte not found";
                default: return "Unknown error while parsing database";
            }
        }
    };
    static const ParseError_category theParseError_category{};
}  // namespace detail

std::error_code make_error_code(ParseError e) noexcept
{
    return { static_cast<int>(e), detail::theParseError_category };
}

enum class bin {
    auto_left_margin,
    auto_right_margin,
    no_esc_ctlc,
    ceol_standout_glitch,
    eat_newline_glitch,
    erase_overstrike,
    generic_type,
    hard_copy,
    has_meta_key,
    has_status_line,
    insert_null_glitch,
    memory_above,
    memory_below,
    move_insert_mode,
    move_standout_mode,
    over_strike,
    status_line_esc_ok,
    dest_tabs_magic_smso,
    tilde_glitch,
    transparent_underline,
    xon_xoff,
    needs_xon_xoff,
    prtr_silent,
    hard_cursor,
    non_rev_rmcup,
    no_pad_char,
    non_dest_scroll_region,
    can_change,
    back_color_erase,
    hue_lightness_saturation,
    col_addr_glitch,
    cr_cancels_micro_mode,
    has_print_wheel,
    row_addr_glitch,
    semi_auto_right_margin,
    cpi_changes_res,
    lpi_changes_res,
    backspaces_with_bs,
    crt_no_scrolling,
    no_correctly_working_cr,
    gnu_has_meta_key,
    linefeed_is_newline,
    has_hardware_tabs,
    return_does_clr_eol
};

enum class num {
    columns,
    init_tabs,
    lines,
    lines_of_memory,
    magic_cookie_glitch,
    padding_baud_rate,
    virtual_terminal,
    width_status_line,
    num_labels,
    label_height,
    label_width,
    max_attributes,
    maximum_windows,
    max_colors,
    max_pairs,
    no_color_video,
    buffer_capacity,
    dot_vert_spacing,
    dot_horz_spacing,
    max_micro_address,
    max_micro_jump,
    micro_col_size,
    micro_line_size,
    number_of_pins,
    output_res_char,
    output_res_line,
    output_res_horz_inch,
    output_res_vert_inch,
    print_rate,
    wide_char_size,
    buttons,
    bit_image_entwining,
    bit_image_type,
    magic_cookie_glitch_ul,
    carriage_return_delay,
    new_line_delay,
    backspace_delay,
    horizontal_tab_delay,
    number_of_function_keys
};

enum class str {
    back_tab,
    bell,
    carriage_return,
    change_scroll_region,
    clear_all_tabs,
    clear_screen,
    clr_eol,
    clr_eos,
    column_address,
    command_character,
    cursor_address,
    cursor_down,
    cursor_home,
    cursor_invisible,
    cursor_left,
    cursor_mem_address,
    cursor_normal,
    cursor_right,
    cursor_to_ll,
    cursor_up,
    cursor_visible,
    delete_character,
    delete_line,
    dis_status_line,
    down_half_line,
    enter_alt_charset_mode,
    enter_blink_mode,
    enter_bold_mode,
    enter_ca_mode,
    enter_delete_mode,
    enter_dim_mode,
    enter_insert_mode,
    enter_secure_mode,
    enter_protected_mode,
    enter_reverse_mode,
    enter_standout_mode,
    enter_underline_mode,
    erase_chars,
    exit_alt_charset_mode,
    exit_attribute_mode,
    exit_ca_mode,
    exit_delete_mode,
    exit_insert_mode,
    exit_standout_mode,
    exit_underline_mode,
    flash_screen,
    form_feed,
    from_status_line,
    init_1string,
    init_2string,
    init_3string,
    init_file,
    insert_character,
    insert_line,
    insert_padding,
    key_backspace,
    key_catab,
    key_clear,
    key_ctab,
    key_dc,
    key_dl,
    key_down,
    key_eic,
    key_eol,
    key_eos,
    key_f0,
    key_f1,
    key_f10,
    key_f2,
    key_f3,
    key_f4,
    key_f5,
    key_f6,
    key_f7,
    key_f8,
    key_f9,
    key_home,
    key_ic,
    key_il,
    key_left,
    key_ll,
    key_npage,
    key_ppage,
    key_right,
    key_sf,
    key_sr,
    key_stab,
    key_up,
    keypad_local,
    keypad_xmit,
    lab_f0,
    lab_f1,
    lab_f10,
    lab_f2,
    lab_f3,
    lab_f4,
    lab_f5,
    lab_f6,
    lab_f7,
    lab_f8,
    lab_f9,
    meta_off,
    meta_on,
    newline,
    pad_char,
    parm_dch,
    parm_delete_line,
    parm_down_cursor,
    parm_ich,
    parm_index,
    parm_insert_line,
    parm_left_cursor,
    parm_right_cursor,
    parm_rindex,
    parm_up_cursor,
    pkey_key,
    pkey_local,
    pkey_xmit,
    print_screen,
    prtr_off,
    prtr_on,
    repeat_char,
    reset_1string,
    reset_2string,
    reset_3string,
    reset_file,
    restore_cursor,
    row_address,
    save_cursor,
    scroll_forward,
    scroll_reverse,
    set_attributes,
    set_tab,
    set_window,
    tab,
    to_status_line,
    underline_char,
    up_half_line,
    init_prog,
    key_a1,
    key_a3,
    key_b2,
    key_c1,
    key_c3,
    prtr_non,
    char_padding,
    acs_chars,
    plab_norm,
    key_btab,
    enter_xon_mode,
    exit_xon_mode,
    enter_am_mode,
    exit_am_mode,
    xon_character,
    xoff_character,
    ena_acs,
    label_on,
    label_off,
    key_beg,
    key_cancel,
    key_close,
    key_command,
    key_copy,
    key_create,
    key_end,
    key_enter,
    key_exit,
    key_find,
    key_help,
    key_mark,
    key_message,
    key_move,
    key_next,
    key_open,
    key_options,
    key_previous,
    key_print,
    key_redo,
    key_reference,
    key_refresh,
    key_replace,
    key_restart,
    key_resume,
    key_save,
    key_suspend,
    key_undo,
    key_sbeg,
    key_scancel,
    key_scommand,
    key_scopy,
    key_screate,
    key_sdc,
    key_sdl,
    key_select,
    key_send,
    key_seol,
    key_sexit,
    key_sfind,
    key_shelp,
    key_shome,
    key_sic,
    key_sleft,
    key_smessage,
    key_smove,
    key_snext,
    key_soptions,
    key_sprevious,
    key_sprint,
    key_sredo,
    key_sreplace,
    key_sright,
    key_srsume,
    key_ssave,
    key_ssuspend,
    key_sundo,
    req_for_input,
    key_f11,
    key_f12,
    key_f13,
    key_f14,
    key_f15,
    key_f16,
    key_f17,
    key_f18,
    key_f19,
    key_f20,
    key_f21,
    key_f22,
    key_f23,
    key_f24,
    key_f25,
    key_f26,
    key_f27,
    key_f28,
    key_f29,
    key_f30,
    key_f31,
    key_f32,
    key_f33,
    key_f34,
    key_f35,
    key_f36,
    key_f37,
    key_f38,
    key_f39,
    key_f40,
    key_f41,
    key_f42,
    key_f43,
    key_f44,
    key_f45,
    key_f46,
    key_f47,
    key_f48,
    key_f49,
    key_f50,
    key_f51,
    key_f52,
    key_f53,
    key_f54,
    key_f55,
    key_f56,
    key_f57,
    key_f58,
    key_f59,
    key_f60,
    key_f61,
    key_f62,
    key_f63,
    clr_bol,
    clear_margins,
    set_left_margin,
    set_right_margin,
    label_format,
    set_clock,
    display_clock,
    remove_clock,
    create_window,
    goto_window,
    hangup,
    dial_phone,
    quick_dial,
    tone,
    pulse,
    flash_hook,
    fixed_pause,
    wait_tone,
    user0,
    user1,
    user2,
    user3,
    user4,
    user5,
    user6,
    user7,
    user8,
    user9,
    orig_pair,
    orig_colors,
    initialize_color,
    initialize_pair,
    set_color_pair,
    set_foreground,
    set_background,
    change_char_pitch,
    change_line_pitch,
    change_res_horz,
    change_res_vert,
    define_char,
    enter_doublewide_mode,
    enter_draft_quality,
    enter_italics_mode,
    enter_leftward_mode,
    enter_micro_mode,
    enter_near_letter_quality,
    enter_normal_quality,
    enter_shadow_mode,
    enter_subscript_mode,
    enter_superscript_mode,
    enter_upward_mode,
    exit_doublewide_mode,
    exit_italics_mode,
    exit_leftward_mode,
    exit_micro_mode,
    exit_shadow_mode,
    exit_subscript_mode,
    exit_superscript_mode,
    exit_upward_mode,
    micro_column_address,
    micro_down,
    micro_left,
    micro_right,
    micro_row_address,
    micro_up,
    order_of_pins,
    parm_down_micro,
    parm_left_micro,
    parm_right_micro,
    parm_up_micro,
    select_char_set,
    set_bottom_margin,
    set_bottom_margin_parm,
    set_left_margin_parm,
    set_right_margin_parm,
    set_top_margin,
    set_top_margin_parm,
    start_bit_image,
    start_char_set_def,
    stop_bit_image,
    stop_char_set_def,
    subscript_characters,
    superscript_characters,
    these_cause_cr,
    zero_motion,
    char_set_names,
    key_mouse,
    mouse_info,
    req_mouse_pos,
    get_mouse,
    set_a_foreground,
    set_a_background,
    pkey_plab,
    device_type,
    code_set_init,
    set0_des_seq,
    set1_des_seq,
    set2_des_seq,
    set3_des_seq,
    set_lr_margin,
    set_tb_margin,
    bit_image_repeat,
    bit_image_newline,
    bit_image_carriage_return,
    color_names,
    define_bit_image_region,
    end_bit_image_region,
    set_color_band,
    set_page_length,
    display_pc_char,
    enter_pc_charset_mode,
    exit_pc_charset_mode,
    enter_scancode_mode,
    exit_scancode_mode,
    pc_term_options,
    scancode_escape,
    alt_scancode_esc,
    enter_horizontal_hl_mode,
    enter_left_hl_mode,
    enter_low_hl_mode,
    enter_right_hl_mode,
    enter_top_hl_mode,
    enter_vertical_hl_mode,
    set_a_attributes,
    set_pglen_inch,
    termcap_init2,
    termcap_reset,
    linefeed_if_not_lf,
    backspace_if_not_bs,
    other_non_function_keys,
    arrow_key_map,
    acs_ulcorner,
    acs_llcorner,
    acs_urcorner,
    acs_lrcorner,
    acs_ltee,
    acs_rtee,
    acs_btee,
    acs_ttee,
    acs_hline,
    acs_vline,
    acs_plus,
    memory_lock,
    memory_unlock,
    box_chars_1
};

enum class Exceptions : bool { ON = true, OFF = false };

template <Exceptions E = Exceptions::ON>
class TermDb {
private:
    std::bitset<numCapBool> booleans{};
    std::array<uint16_t, numCapNum> numbers{};
    std::vector<uint16_t> stringOffset;
    std::vector<char> stringTable;

    std::string name;
    bool isValidState = false;

    // https://regex101.com/r/GwGLfk/1
    static constexpr auto delayStr = R"(\$<\d+(\.\d{1})?((\/?\*?)|(\*?\/?))>)";

    std::error_code loadDB(const std::string, std::string);
    void escape(std::string &) const;
    std::string parser(const std::string &s, param p1, param p2, param p3,
                       param p4, param p5, param p6, param p7, param p8,
                       param p9) const;
    bool parse(const std::string, std::string = DPATH);

#if defined(OS_WIN)
    bool isLessThanWin10 = false;
    HANDLE consoleHandle = nullptr;

    void _emulate_clear() const noexcept;

    std::string do_clear() const noexcept;
#endif

public:
    TermDb();

    TermDb(const std::string &_name, std::string _path = DPATH);

    explicit operator bool() const noexcept;
    std::string getName() const { return name; }


    bool get(tdb::bin _b) const noexcept
    {
        const auto b = static_cast<int>(_b);
        return booleans[b];
    }

    nonstd::optional<uint16_t> get(tdb::num _n) const noexcept
    {
        // NP represents 'Not Present' properties, represented by
        // -1 value in terminfo databases.
        const auto n      = static_cast<int>(_n);
        const auto result = numbers[n];
        if (result == (std::numeric_limits<uint16_t>::max)()) {
            return {};
        } else {
            return result;
        }
    }


    std::string get(tdb::str _s, param p1 = 0l, param p2 = 0l, param p3 = 0l,
                    param p4 = 0l, param p5 = 0l, param p6 = 0l, param p7 = 0l,
                    param p8 = 0l, param p9 = 0l) const
    {
#if defined(OS_LINUX) || defined(OS_MAC)
        static const std::regex pattern(delayStr, std::regex::optimize);

        const size_t s = static_cast<int>(_s);
        std::string result;

        if (!stringOffset.empty() && s < stringOffset.size()) {
            const auto offset      = stringOffset[s];
            constexpr auto INVALID = (std::numeric_limits<uint16_t>::max)();
            if (offset != INVALID && offset < stringTable.size()) {
                result.append(&stringTable[offset]);
                escape(result);
                result = std::regex_replace(result, pattern, "");
                result = parser(result, p1, p2, p3, p4, p5, p6, p7, p8, p9);
            }
        }
        return result;

#elif defined(OS_WIN)
        if (!isValidState) {
            return "";
        }

        switch (_s) {
            case tdb::str::clear_screen: return do_clear();
            default: return "";
        }
#endif
    }
};

template<>
TermDb<Exceptions::OFF>::TermDb(){
#if defined(OS_WIN)
    name         = "cmd.exe";
    isValidState = true;
#elif defined(OS_LINUX) || defined(OS_MAC)
    const auto terminalToBe = std::getenv("TERM");
    if (terminalToBe) {
        const auto error = loadDB(terminalToBe, DPATH);
        isValidState = error ? false : true;
    } else {
        isValidState = false;
    }
#endif
}

template<>
TermDb<Exceptions::ON>::TermDb(){
#if defined(OS_WIN)
    name         = "cmd.exe";
    isValidState = true;
#elif defined(OS_LINUX) || defined(OS_MAC)
    const auto terminalToBe = std::getenv("TERM");
    if (terminalToBe) {
        const auto error = loadDB(terminalToBe, DPATH);
        if (error) {
            throw error;
        } else {
            isValidState = true;
        }
    } else {
        throw;
    }
#endif
}

template <>
TermDb<Exceptions::OFF>::TermDb(const std::string &_name, std::string _path)
{
    const auto error = loadDB(_name, _path);
    isValidState = error ? false : true;
}

template <>
TermDb<Exceptions::ON>::TermDb(const std::string &_name, std::string _path)
{
#if defined(OS_WIN)
    if (_path.empty()) {
        name         = "cmd.exe";
        isValidState = true;
    } else {
#endif

        const auto error = loadDB(_name, _path);
        if (error) {
            throw error;
        } else {
            isValidState = true;
        }

#if defined(OS_WIN)
    }
#endif
}

template<>
TermDb<Exceptions::OFF>::operator bool() const noexcept { return isValidState; }

template<>
TermDb<Exceptions::ON>::operator bool() const noexcept = delete;

template <Exceptions E>
std::error_code TermDb<E>::loadDB(const std::string _name, std::string _path)
{
    const auto hashCharacter = [](unsigned char c) {
        if (c < 10) {
            return c + '0';
        } else {
            return (c - 10) + 'a';
        }
    };

    std::error_code ec = tdb::ParseError::Success;
    if (_name.empty() || _path.empty()) {
        ec = tdb::ParseError::ReadError;
        return ec;
    }


    std::string tryPath = _path;
    char separator;
#if defined(OS_LINUX) || defined(OS_MAC)
    separator = '/';
#elif defined(OS_WIN)
    separator = '\\';
#endif

    tryPath.append(_name, 0, 1).append(1, separator).append(_name);

    std::ifstream db(tryPath.c_str(), std::ios::binary | std::ios::ate);
    if (!db) {
        // try using hash value
        char hash[2];
        unsigned char firstchar = _name[0];

        hash[0] = hashCharacter((firstchar & 0xF0) >> 4);
        hash[1] = hashCharacter(firstchar & 0x0F);
        _path.append(&hash[0], 2).append(1, separator).append(_name);

        db.clear();
        db.open(_path, std::ios::binary | std::ios::ate);

        if (!db) {
            ec = tdb::ParseError::ReadError;
            return ec;
        }
    }

    const int size = db.tellg();
    if (size == 0) {
        ec = tdb::ParseError::BadDatabase;
        return ec;
    }


    db.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    db.read(reinterpret_cast<char *>(buffer.data()), size);
    db.close();

    if (db.fail()) {
        ec = tdb::ParseError::ReadError;
        return ec;
    }


    // sanitize if no at-the-end null byte present
    buffer.push_back('\0');

    // header contains a constant magic number
    const auto magic_byte = buffer[0] | (buffer[1] << 8);
    if (magic_byte != 0432) {
        ec = tdb::ParseError::MagicByteError;
        return ec;
    }


    /* size list contains size/numbers of -
    - name [0]
    - booleans [1]
    - numbers [2]
    - offsets [3]
    - stringTable [4]
    */
    uint16_t sList[5] = { 0 };
    for (auto i = 2, j = 0; i < 12; i += 2, ++j) {
        sList[j] = buffer[i] | (buffer[i + 1] << 8);
    }


    // check for malformed databases
    const std::size_t minBytes
      = 12 + sList[0] + sList[1] + ((sList[2] + sList[3]) * 2) + sList[4];
    if (buffer.size() <= minBytes) {
        ec = tdb::ParseError::BadDatabase;
        return ec;
    }

    // parse name of terms
    name.append(buffer.begin() + 12, buffer.begin() + 11 + sList[0]);

    // parse boolean values
    int cursor = 12 + sList[0];
    for (auto i = cursor; i < cursor + sList[1]; ++i) {
        if (buffer[i]) {
            booleans.set(i - cursor);
        }
    }
    cursor += sList[1];


    /*
    Between the boolean section and the number section, a null
    byte will be inserted, if necessary, to ensure that the
    number section begins on an even byte (this is a relic of
    the PDP-11's word-addressed architecture, originally
    designed in to avoid IOT traps induced by addressing a
    word on an odd byte boundary). All short integers are
    aligned on a short word boundary.
    */
    cursor += cursor % 2;


    // parse numbers values
    for (auto i = cursor, j = 0; j < sList[2]; i += 2, ++j) {
        numbers[j] = buffer[i] | (buffer[i + 1] << 8);
    }
    cursor += sList[2] * 2;


    // parse stringOffset values
    stringOffset.resize(sList[3], 0);
    for (auto i = cursor, j = 0; j < sList[3]; i += 2, ++j) {
        stringOffset[j] = buffer[i] | (buffer[i + 1] << 8);
    }
    cursor += sList[3] * 2;


    // move rest of the buffer to string table
    std::move(buffer.begin() + cursor, buffer.end(),
              std::inserter(stringTable, stringTable.begin()));

    return ec;
}

template <Exceptions E>
void TermDb<E>::escape(std::string &input) const
{
    const auto isDigit = [](const char c) { return (c >= '0' && c <= '9'); };

    std::string result;
    auto strLength   = input.length();
    bool activeParse = false;
    errno            = 0;

    for (decltype(strLength) i = 0; i < strLength; ++i) {

        if (!activeParse) {
            if (input[i] == '\\') {
                activeParse = true;
            } else {
                result += input[i];
            }
            continue;
        }

        switch (input[i]) {
            case '^': result += '^'; break;
            case '\\': result += '\\'; break;
            case ',': result += ','; break;
            case ':': result += ':'; break;
            case '0':
                if ((i + 1) == strLength || !isDigit(input[i + 1])) {
                    result += "\\200";
                    break;
                }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                if ((i + 2) < strLength && isDigit(input[i + 1])
                    && isDigit(input[i + 2])) {
                    char arr[4];
                    arr[0] = input[i];
                    arr[1] = input[i + 1];
                    arr[2] = input[i + 2];
                    arr[3] = 0;
                    i += 2;
                    const auto decNum = strtol(arr, NULL, 8);
                    if (errno != 0) {
                        result.clear();
                    } else {
                        result += std::to_string(decNum);
                    }
                    break;
                }
            default: result += input[i]; break;
        }
        activeParse = false;
    }
    std::swap(result, input);
}

template <Exceptions E>
std::string TermDb<E>::parser(const std::string &s, param p1, param p2,
                              param p3, param p4, param p5, param p6, param p7,
                              param p8, param p9) const
{
    const auto isDigit    = [](const char c) { return (c >= '0' && c <= '9'); };
    const auto isFlagChar = [](const char c) {
        return (c == '-' || c == '+' || c == '#' || c == ' ');
    };
    const auto isAlpha = [](const char c) {
        return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
    };
    const auto isPopChar = [](const char c) {
        return (c == 'd' || c == 'o' || c == 'x' || c == 'X');
    };

    class stkOfParams {
        std::stack<param, std::vector<param>> stk;

    public:
        nonstd::optional<long> popNum() noexcept
        {
            if (!stk.empty()) {
                auto &element = stk.top();
                try {
                    auto num = mpark::get<long>(element);
                    stk.pop();
                    return num;
                } catch (mpark::bad_variant_access &err) {
                    return {};
                }
            } else {
                return {};
            }
        }

        nonstd::optional<std::string> popString() noexcept
        {
            if (!stk.empty()) {
                auto &element = stk.top();
                try {
                    auto num = mpark::get<std::string>(element);
                    stk.pop();
                    return num;
                } catch (mpark::bad_variant_access &err) {
                    return {};
                }
            } else {
                return {};
            }
        }

        nonstd::optional<param> pop() noexcept
        {
            if (!stk.empty()) {
                const auto element = stk.top();
                stk.pop();
                return element;
            } else {
                return {};
            }
        }

        void push(const param _value) noexcept { stk.push(_value); }
    };

    struct Variables {
        std::array<param, 26> small;
        std::array<param, 26> big;
    };

    struct Context {
        bool hasThen         = false;
        bool hasElse         = false;
        bool ignoreMode      = false;
        bool scopeIgnoreMode = false;
        Context(const bool _sim) : scopeIgnoreMode(_sim) {}
        Context() = delete;
    };

    std::string result;
    stkOfParams stk;
    std::stack<Context, std::vector<Context>> conList;
    std::ostringstream oss;
    static Variables V{};

    bool activeParse     = false;
    bool incorrectString = false;
    auto strLength       = s.length();

    bool prependBase  = false;
    bool prependSpace = false;
    auto precision    = std::string::npos;

    for (decltype(strLength) i = 0; i < strLength; ++i) {
        // checks if % encoding has started
        if (!activeParse) {
            if (s[i] == '%') {
                activeParse = true;
            } else {
                if (conList.empty()
                    || (!conList.empty() && !conList.top().scopeIgnoreMode
                        && !conList.top().ignoreMode)) {
                    result += s[i];
                }
            }
            continue;
        }

        // % encoding has started from here

        // conditional operations
        bool isConditional = true;
        switch (s[i]) {
            case '?':
                conList.emplace(conList.empty() ? false
                                                : conList.top().ignoreMode);
                break;

            case 't': {
                if (conList.empty()) {
                    incorrectString = true;
                    break;
                }
                if (conList.top().scopeIgnoreMode) {
                    break;
                }

                auto p = stk.popNum();
                if (p) {
                    conList.top().hasThen    = p.value() != 0;
                    conList.top().ignoreMode = p.value() == 0;
                } else {
                    incorrectString = true;
                }
                break;
            }

            case 'e':
                if (conList.empty()) {
                    incorrectString = true;
                    break;
                }
                if (conList.top().scopeIgnoreMode) {
                    break;
                }
                conList.top().ignoreMode = conList.top().hasThen;
                break;

            case ';':
                if (!conList.empty()) {
                    conList.pop();
                } else {
                    incorrectString = true;
                }
                break;

            default: isConditional = false; break;
        }

        if (incorrectString) {
            break;
        }

        if (isConditional
            || (!conList.empty()
                && (conList.top().scopeIgnoreMode
                    || conList.top().ignoreMode))) {
            activeParse = false;
            continue;
        }

        // non-condition operations
        switch (s[i]) {
            case '%': result += '%'; break;

            case ':': {
            FLAGS_LABEL:
                if (++i >= strLength) {
                    incorrectString = true;
                    break;
                }
                switch (s[i]) {
                    case '-': oss << std::left; break;
                    case '#': prependBase = true; break;
                    case '+':
                    case ' ':
                        oss << std::showpos;
                        prependSpace = (s[i] == ' ');
                        break;
                    default: incorrectString = true; break;
                }

                if ((++i) < strLength && s[i] != s[i - 1]) {
                    if (isDigit(s[i])) {
                        goto WIDTH_LABEL;
                    } else if (s[i] == '.') {
                        goto PRECISION_LABEL;
                    } else if (isPopChar(s[i])) {
                        goto POP_LABEL;
                    } else if (isFlagChar(s[i])) {
                        goto FLAGS_LABEL;
                    } else {
                        incorrectString = true;
                    }
                } else {
                    incorrectString = true;
                }

                break;
            }

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
            WIDTH_LABEL:
                int w = s[i] - '0';
                while ((i + 1) < strLength && isDigit(s[i + 1])) {
                    ++i;
                    w = (w * 10) + (s[i] - '0');
                }
                oss << std::setw(w);

                if ((i + 1) < strLength) {
                    if (s[i + 1] == '.') {
                        ++i;
                        goto PRECISION_LABEL;
                    } else if (isPopChar(s[i])) {
                        ++i;
                        goto POP_LABEL;
                    } else {
                        incorrectString = true;
                    }
                } else {
                    incorrectString = true;
                }
                break;
            }

            case '.': {
            PRECISION_LABEL:
                if (++i >= strLength && !isDigit(s[i])) {
                    incorrectString = true;
                    break;
                }

                int p = s[i] - '0';
                while ((i + 1) < strLength && isDigit(s[i + 1])) {
                    ++i;
                    p = (p * 10) + (s[i] - '0');
                }
                precision = p;

                if ((i + 1) < strLength && isPopChar(s[i])) {
                    ++i;
                    goto POP_LABEL;
                } else {
                    incorrectString = true;
                }
                break;
            }

            case 'd':
            case 'o':
            case 'x':
            case 'X': {
            POP_LABEL:
                auto success = stk.popNum();
                if (success) {
                    switch (s[i]) {
                        case 'o':
                            oss << (prependBase ? "0" : "") << std::oct;
                            break;
                        case 'x':
                            oss << (prependBase ? "0x" : "") << std::hex;
                            break;
                        case 'X':
                            oss << (prependBase ? "0X" : "") << std::hex;
                            break;
                    }
                    oss << success.value();
                    auto str = prependSpace
                      ? oss.str().replace(oss.str().find('+'), 1, 1, ' ')
                      : oss.str();
                    result += str.substr(0, precision);
                    oss.clear();
                    oss.str("");
                    prependBase  = false;
                    prependSpace = false;
                    precision    = std::string::npos;
                } else {
                    incorrectString = true;
                }
                break;
            }

            case 'c': {
                auto success = stk.popNum();
                if (success) {
                    result += static_cast<char>(success.value());
                } else {
                    incorrectString = true;
                }
                break;
            }

            case 's': {
                auto success = stk.popString();
                if (success) {
                    result += success.value();
                } else {
                    incorrectString = true;
                }
                break;
            }

            case 'p': {
                if (++i >= strLength) {
                    incorrectString = true;
                    break;
                }

                switch (s[i]) {
                    case '1': stk.push(p1); break;
                    case '2': stk.push(p2); break;
                    case '3': stk.push(p3); break;
                    case '4': stk.push(p4); break;
                    case '5': stk.push(p5); break;
                    case '6': stk.push(p6); break;
                    case '7': stk.push(p7); break;
                    case '8': stk.push(p8); break;
                    case '9': stk.push(p9); break;
                    default: incorrectString = true; break;
                }
                break;
            }

            case 'l': {
                auto success = stk.popString();
                if (success) {
                    stk.push(success.value().length());
                } else {
                    incorrectString = true;
                }
                break;
            }

            case 'i': {
                try {
                    auto &param1 = mpark::get<long>(p1);
                    auto &param2 = mpark::get<long>(p2);
                    ++param1;
                    ++param2;
                } catch (mpark::bad_variant_access &err) {
                    incorrectString = true;
                }
                break;
            }

            case '!':
            case '~': {
                auto success = stk.popNum();
                if (success) {
                    if (s[i] == '!') {
                        stk.push(!(success.value()));
                    } else {
                        stk.push(~(success.value()));
                    }
                } else {
                    incorrectString = true;
                }
                break;
            }

            case '\'': {
                if (++i >= strLength) {
                    incorrectString = true;
                    break;
                }
                char ch = s[i];
                if (++i >= strLength || s[i] != '\'') {
                    incorrectString = true;
                    break;
                }
                stk.push(ch);
                ++i;
                break;
            }

            case '{': {
                if (++i >= strLength || !isdigit(s[i])) {
                    incorrectString = true;
                    break;
                }
                int num = s[i] - '0';
                if (++i >= strLength || !isdigit(s[i])) {
                    if (s[i] == '}') {
                        stk.push(num);
                    } else {
                        incorrectString = true;
                    }
                    break;
                }
                num = (num * 10) + (s[i] - '0');
                if (++i >= strLength || s[i] != '}') {
                    incorrectString = true;
                    break;
                }
                stk.push(num);
                break;
            }

            case '+':
            case '-':
            case '*':
            case '/':
            case 'm':
            case '&':
            case '|':
            case '^':
            case '=':
            case '>':
            case '<':
            case 'A':
            case 'O': {
                auto num1 = stk.popNum();
                if (!num1) {
                    incorrectString = true;
                    break;
                }
                auto num2 = stk.popNum();
                if (!num2) {
                    incorrectString = true;
                    break;
                }

                switch (s[i]) {
                    case '+': stk.push(num1.value() + num2.value()); break;
                    case '-': stk.push(num1.value() - num2.value()); break;
                    case '*': stk.push(num1.value() * num2.value()); break;
                    case '/': stk.push(num1.value() / num2.value()); break;
                    case 'm': stk.push(num1.value() % num2.value()); break;
                    case '&': stk.push(num1.value() & num2.value()); break;
                    case '|': stk.push(num1.value() | num2.value()); break;
                    case '^': stk.push(num1.value() ^ num2.value()); break;
                    case '=': stk.push(num1.value() == num2.value()); break;
                    case '>': stk.push(num1.value() > num2.value()); break;
                    case '<': stk.push(num1.value() < num2.value()); break;
                    case 'A': stk.push(num1.value() && num2.value()); break;
                    case 'O': stk.push(num1.value() || num2.value()); break;
                    default: incorrectString = true; break;
                }
                break;
            }

            case 'P': {
                if (++i >= strLength || !isAlpha(s[i])) {
                    incorrectString = true;
                    break;
                }
                if (s[i] >= 'A' && s[i] <= 'Z') {
                    auto index   = s[i] - 'A';
                    auto success = stk.pop();
                    if (success) {
                        V.big[index] = success.value();
                    } else {
                        incorrectString = true;
                    }
                } else if (s[i] >= 'a' && s[i] <= 'z') {
                    auto index   = s[i] - 'a';
                    auto success = stk.pop();
                    if (success) {
                        V.small[index] = success.value();
                    } else {
                        incorrectString = true;
                    }
                } else {
                    incorrectString = true;
                }
                break;
            }

            case 'g': {
                if (++i >= strLength || !isAlpha(s[i])) {
                    incorrectString = true;
                    break;
                }
                if (s[i] >= 'A' && s[i] <= 'Z') {
                    const auto index = s[i] - 'A';
                    stk.push(V.big[index]);
                } else if (s[i] >= 'a' && s[i] <= 'z') {
                    const auto index = s[i] - 'a';
                    stk.push(V.small[index]);
                } else {
                    incorrectString = true;
                }
                break;
            }

            default: incorrectString = true; break;
        }

        if (incorrectString) {
            break;
        }
        activeParse = false;
    }
    return incorrectString ? "" : result;
}


template <Exceptions E>
bool TermDb<E>::parse(const std::string _name, std::string _path)
{

    name.clear();
    booleans.reset();
    numbers.fill((std::numeric_limits<uint16_t>::max)());
    stringOffset.clear();
    stringTable.clear();

    const auto error = loadDB(_name, _path);
    isValidState     = error ? false : true;

    return isValidState;
}

#if defined(OS_WIN)
template <Exceptions E>
void TermDb<E>::_emulate_clear() const noexcept
{
    CONSOLE_SCREEN_BUFFER_INFO buffInfo;
    GetConsoleScreenBufferInfo(consoleHandle, &buffInfo);

    DWORD totalChars   = buffInfo.dwSize.X * buffInfo.dwSize.Y;
    COORD startPos     = { 0, 0 };
    DWORD charsWritten = 0;

    FillConsoleOutputCharacter(consoleHandle, ' ', totalChars, startPos,
                               &charsWritten);
    FillConsoleOutputAttribute(consoleHandle, buffInfo.wAttributes, totalChars,
                               startPos,
                               &charsWritten);  // Resetting attributes
    SetConsoleCursorPosition(consoleHandle,
                             startPos);  // Position cursor to home
}

template <Exceptions E>
std::string TermDb<E>::do_clear() const noexcept
{
    if (isLessThanWin10) {
        _emulate_clear();
        return "";
    } else {
        return "\x1b[2J";
    }
}
#endif

}  // namespace tdb

#endif
