// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2018-2020 Bitcoin Association
// Distributed under the Open BSV software license, see the accompanying file
// LICENSE.

#include "script.h"
#include "consensus/consensus.h"
#include "instruction_iterator.h"
#include "int_serialization.h"
#include "script_num.h"

#include "tinyformat.h"
#include "utilstrencodings.h"

#include <algorithm>
#include <sstream>

std::ostream& operator<<(std::ostream& os, const opcodetype& opcode)
{
    if(opcode >= 1 && opcode <= 75)
    {
        os << static_cast<int>(opcode);
        return os;
    }

    switch(opcode)
    {
    case OP_0:
        os << "OP_0";
        break;
    case OP_1:
        os << "OP_1";
        break;
    case OP_2:
        os << "OP_2";
        break;
    case OP_3:
        os << "OP_3";
        break;
    case OP_4:
        os << "OP_4";
        break;
    case OP_5:
        os << "OP_5";
        break;
    case OP_6:
        os << "OP_6";
        break;
    case OP_7:
        os << "OP_7";
        break;
    case OP_8:
        os << "OP_8";
        break;
    case OP_9:
        os << "OP_9";
        break;
    case OP_10:
        os << "OP_10";
        break;
    case OP_11:
        os << "OP_11";
        break;
    case OP_12:
        os << "OP_12";
        break;
    case OP_13:
        os << "OP_13";
        break;
    case OP_14:
        os << "OP_14";
        break;
    case OP_15:
        os << "OP_15";
        break;
    case OP_16:
        os << "OP_16";
        break;
    default:
        os << GetOpName(opcode);
    }
    return os;
}

const char *GetOpName(opcodetype opcode) {
    switch (opcode) {
        // push value
        case OP_0:
            return "0";
        case OP_PUSHDATA1:
            return "OP_PUSHDATA1";
        case OP_PUSHDATA2:
            return "OP_PUSHDATA2";
        case OP_PUSHDATA4:
            return "OP_PUSHDATA4";
        case OP_1NEGATE:
            return "-1";
        case OP_RESERVED:
            return "OP_RESERVED";
        case OP_1:
            return "1";
        case OP_2:
            return "2";
        case OP_3:
            return "3";
        case OP_4:
            return "4";
        case OP_5:
            return "5";
        case OP_6:
            return "6";
        case OP_7:
            return "7";
        case OP_8:
            return "8";
        case OP_9:
            return "9";
        case OP_10:
            return "10";
        case OP_11:
            return "11";
        case OP_12:
            return "12";
        case OP_13:
            return "13";
        case OP_14:
            return "14";
        case OP_15:
            return "15";
        case OP_16:
            return "16";

        // control
        case OP_NOP:
            return "OP_NOP";
        case OP_VER:
            return "OP_VER";
        case OP_IF:
            return "OP_IF";
        case OP_NOTIF:
            return "OP_NOTIF";
        case OP_VERIF:
            return "OP_VERIF";
        case OP_VERNOTIF:
            return "OP_VERNOTIF";
        case OP_ELSE:
            return "OP_ELSE";
        case OP_ENDIF:
            return "OP_ENDIF";
        case OP_VERIFY:
            return "OP_VERIFY";
        case OP_RETURN:
            return "OP_RETURN";

        // stack ops
        case OP_TOALTSTACK:
            return "OP_TOALTSTACK";
        case OP_FROMALTSTACK:
            return "OP_FROMALTSTACK";
        case OP_2DROP:
            return "OP_2DROP";
        case OP_2DUP:
            return "OP_2DUP";
        case OP_3DUP:
            return "OP_3DUP";
        case OP_2OVER:
            return "OP_2OVER";
        case OP_2ROT:
            return "OP_2ROT";
        case OP_2SWAP:
            return "OP_2SWAP";
        case OP_IFDUP:
            return "OP_IFDUP";
        case OP_DEPTH:
            return "OP_DEPTH";
        case OP_DROP:
            return "OP_DROP";
        case OP_DUP:
            return "OP_DUP";
        case OP_NIP:
            return "OP_NIP";
        case OP_OVER:
            return "OP_OVER";
        case OP_PICK:
            return "OP_PICK";
        case OP_ROLL:
            return "OP_ROLL";
        case OP_ROT:
            return "OP_ROT";
        case OP_SWAP:
            return "OP_SWAP";
        case OP_TUCK:
            return "OP_TUCK";

        // splice ops
        case OP_CAT:
            return "OP_CAT";
        case OP_SPLIT:
            return "OP_SPLIT";
        case OP_NUM2BIN:
            return "OP_NUM2BIN";
        case OP_BIN2NUM:
            return "OP_BIN2NUM";
        case OP_SIZE:
            return "OP_SIZE";

        // bit logic
        case OP_INVERT:
            return "OP_INVERT";
        case OP_AND:
            return "OP_AND";
        case OP_OR:
            return "OP_OR";
        case OP_XOR:
            return "OP_XOR";
        case OP_EQUAL:
            return "OP_EQUAL";
        case OP_EQUALVERIFY:
            return "OP_EQUALVERIFY";
        case OP_RESERVED1:
            return "OP_RESERVED1";
        case OP_RESERVED2:
            return "OP_RESERVED2";

        // numeric
        case OP_1ADD:
            return "OP_1ADD";
        case OP_1SUB:
            return "OP_1SUB";
        case OP_2MUL:
            return "OP_2MUL";
        case OP_2DIV:
            return "OP_2DIV";
        case OP_NEGATE:
            return "OP_NEGATE";
        case OP_ABS:
            return "OP_ABS";
        case OP_NOT:
            return "OP_NOT";
        case OP_0NOTEQUAL:
            return "OP_0NOTEQUAL";
        case OP_ADD:
            return "OP_ADD";
        case OP_SUB:
            return "OP_SUB";
        case OP_MUL:
            return "OP_MUL";
        case OP_DIV:
            return "OP_DIV";
        case OP_MOD:
            return "OP_MOD";
        case OP_LSHIFT:
            return "OP_LSHIFT";
        case OP_RSHIFT:
            return "OP_RSHIFT";
        case OP_BOOLAND:
            return "OP_BOOLAND";
        case OP_BOOLOR:
            return "OP_BOOLOR";
        case OP_NUMEQUAL:
            return "OP_NUMEQUAL";
        case OP_NUMEQUALVERIFY:
            return "OP_NUMEQUALVERIFY";
        case OP_NUMNOTEQUAL:
            return "OP_NUMNOTEQUAL";
        case OP_LESSTHAN:
            return "OP_LESSTHAN";
        case OP_GREATERTHAN:
            return "OP_GREATERTHAN";
        case OP_LESSTHANOREQUAL:
            return "OP_LESSTHANOREQUAL";
        case OP_GREATERTHANOREQUAL:
            return "OP_GREATERTHANOREQUAL";
        case OP_MIN:
            return "OP_MIN";
        case OP_MAX:
            return "OP_MAX";
        case OP_WITHIN:
            return "OP_WITHIN";

        // crypto
        case OP_RIPEMD160:
            return "OP_RIPEMD160";
        case OP_SHA1:
            return "OP_SHA1";
        case OP_SHA256:
            return "OP_SHA256";
        case OP_HASH160:
            return "OP_HASH160";
        case OP_HASH256:
            return "OP_HASH256";
        case OP_CODESEPARATOR:
            return "OP_CODESEPARATOR";
        case OP_CHECKSIG:
            return "OP_CHECKSIG";
        case OP_CHECKSIGVERIFY:
            return "OP_CHECKSIGVERIFY";
        case OP_CHECKMULTISIG:
            return "OP_CHECKMULTISIG";
        case OP_CHECKMULTISIGVERIFY:
            return "OP_CHECKMULTISIGVERIFY";

        // expansion
        case OP_NOP1:
            return "OP_NOP1";
        case OP_CHECKLOCKTIMEVERIFY:
            return "OP_CHECKLOCKTIMEVERIFY";
        case OP_CHECKSEQUENCEVERIFY:
            return "OP_CHECKSEQUENCEVERIFY";
        case OP_NOP4:
            return "OP_NOP4";
        case OP_NOP5:
            return "OP_NOP5";
        case OP_NOP6:
            return "OP_NOP6";
        case OP_NOP7:
            return "OP_NOP7";
        case OP_NOP8:
            return "OP_NOP8";
        case OP_NOP9:
            return "OP_NOP9";
        case OP_NOP10:
            return "OP_NOP10";

        case OP_INVALIDOPCODE:
            return "OP_INVALIDOPCODE";

        // Note:
        //  The template matching params OP_SMALLINTEGER/etc are defined in
        //  opcodetype enum as kind of implementation hack, they are *NOT*
        //  real opcodes. If found in real Script, just let the default:
        //  case deal with them.

        default:
            return "OP_UNKNOWN";
    }
}

uint64_t CScript::GetSigOpCount(bool fAccurate, bool isGenesisEnabled, bool& sigOpCountError) const
{
    sigOpCountError = false;
    uint64_t n = 0;
    bsv::instruction last_instruction{OP_INVALIDOPCODE};
    const auto it_end{end_instructions()};
    for(auto it{begin_instructions()}; it != it_end; ++it)
    {
        opcodetype lastOpcode{last_instruction.opcode()};

        opcodetype opcode{it->opcode()};
        if(it->opcode() == OP_INVALIDOPCODE)
            break;

        if(opcode == OP_CHECKSIG || opcode == OP_CHECKSIGVERIFY)
        {
            n++;
        }
        else if(opcode == OP_CHECKMULTISIG || 
            opcode == OP_CHECKMULTISIGVERIFY)
        {
            if ((fAccurate || isGenesisEnabled) && lastOpcode >= OP_1 && lastOpcode <= OP_16)
            {
                n += DecodeOP_N(lastOpcode);
            }
            // post Genesis we always count accurate ops because it's not significantly costlier
            else if (isGenesisEnabled)
            {
                if (lastOpcode == OP_0) 
                {
                    // Checking multisig with 0 keys, so nothing to add to n
                }
                else if(last_instruction.operand().size() > CScriptNum::MAXIMUM_ELEMENT_SIZE)
                {
                    // When trying to spend such output EvalScript does not allow numbers bigger than 4 bytes
                    // and the execution of such script would fail and make the coin unspendable
                    sigOpCountError = true;
                    return 0;
                }
                else
                {
                    //  When trying to spend such output EvalScript requires minimal encoding
                    //  and would fail the script if number is not minimally encoded
                    //  We check minimal encoding before calling CScriptNum to avoid
                    //  exception in CScriptNum constructor.
                    if(!bsv::IsMinimallyEncoded(
                           last_instruction.operand(),
                           CScriptNum::MAXIMUM_ELEMENT_SIZE))
                    {
                        sigOpCountError = true;
                        return 0;
                    }

                    int numSigs =
                        CScriptNum(last_instruction.operand(), true).getint();
                    if(numSigs < 0)
                    {
                        sigOpCountError = true;
                        return 0;
                    }
                    n += numSigs;
                }
            }
            else
            {
                n += MAX_PUBKEYS_PER_MULTISIG_BEFORE_GENESIS;
            }
        }
        last_instruction = *it;
    }

    return n;
}

uint64_t CScript::GetSigOpCount(const CScript &scriptSig, bool isGenesisEnabled, bool& sigOpCountError) const 
{
    sigOpCountError = false;
    if (!IsPayToScriptHash())
    {
        return GetSigOpCount(true, isGenesisEnabled, sigOpCountError);
    }

    // This is a pay-to-script-hash scriptPubKey;
    // get the last item that the scriptSig
    // pushes onto the stack:
    bsv::span<const uint8_t> data;
    const bool valid_script = all_of(scriptSig.begin_instructions(), scriptSig.end_instructions(),
            [&data](const auto& inst)
            {
                if((inst.opcode() > OP_16) || (inst.opcode() == OP_INVALIDOPCODE))  
                    return false;

                data = inst.operand(); 
                return true;
            });
    if(!valid_script)
        return 0;

    if (isGenesisEnabled)
    {
        // After Genesis P2SH is not supported and redeem script is not executed, so we return 0
        return 0;
    }
    else
    {
        /// ... and return its opcount:
        CScript subscript(data.begin(), data.end());
        return subscript.GetSigOpCount(true, isGenesisEnabled, sigOpCountError);
    }
}

bool CScript::IsPayToScriptHash() const {
    // Extra-fast test for pay-to-script-hash CScripts:
    return (this->size() == 23 && (*this)[0] == OP_HASH160 &&
            (*this)[1] == 0x14 && (*this)[22] == OP_EQUAL);
}

// A witness program is any valid CScript that consists of a 1-byte push opcode
// followed by a data push between 2 and 40 bytes.
bool CScript::IsWitnessProgram(int &version,
                               std::vector<uint8_t> &program) const {
    if (this->size() < 4 || this->size() > 42) {
        return false;
    }
    if ((*this)[0] != OP_0 && ((*this)[0] < OP_1 || (*this)[0] > OP_16)) {
        return false;
    }
    if ((size_t)((*this)[1] + 2) == this->size()) {
        version = DecodeOP_N((opcodetype)(*this)[0]);
        program = std::vector<uint8_t>(this->begin() + 2, this->end());
        return true;
    }
    return false;
}

bool CScript::IsPushOnly(const_iterator pc) const {
    while (pc < end()) {
        opcodetype opcode;
        if (!GetOp(pc, opcode)) return false;
        // Note that IsPushOnly() *does* consider OP_RESERVED to be a push-type
        // opcode, however execution of OP_RESERVED fails, so it's not relevant
        // to P2SH/BIP62 as the scriptSig would fail prior to the P2SH special
        // validation code being executed.
        if (opcode > OP_16) return false;
    }
    return true;
}

bool CScript::IsPushOnly() const {
    return this->IsPushOnly(begin());
}

CScript &CScript::push_int64(int64_t n) {
    if (n == -1 || (n >= 1 && n <= 16)) {
        push_back(n + (OP_1 - 1));
    } else if (n == 0) {
        push_back(OP_0);
    } else {
        std::vector<uint8_t> v;
        v.reserve(sizeof(n));
        bsv::serialize(n, back_inserter(v));
        *this << v;
    }
    return *this;
}

CScript &CScript::operator<<(const CScriptNum &b) {
    *this << b.getvch();
    return *this;
}

bsv::instruction_iterator CScript::begin_instructions() const
{
    return bsv::instruction_iterator{bsv::span<const uint8_t>{data(), size()}};
}

bsv::instruction_iterator CScript::end_instructions() const
{
    return bsv::instruction_iterator{
        bsv::span<const uint8_t>{data() + size(), 0}};
}

std::string CScriptWitness::ToString() const {
    std::string ret = "CScriptWitness(";
    for (unsigned int i = 0; i < stack.size(); i++) {
        if (i) {
            ret += ", ";
        }
        ret += HexStr(stack[i]);
    }
    return ret + ")";
}

std::ostream& operator<<(std::ostream& os, const CScript& script)
{
    for(auto it = script.begin_instructions(); it != script.end_instructions();
        ++it)
    {
        os << *it << '\n';
    }

    return os;
}

// used for debugging and pretty-printing in gdb
std::string to_string(const CScript& s)
{
    std::ostringstream oss;
    oss << s;
    return oss.str();
}

size_t CountOp(const bsv::span<const uint8_t> s, const opcodetype opcode)
{
    using namespace bsv;
    instruction_iterator first{s};
    instruction_iterator last{s.last(0)};
    return std::count_if(first, last, [opcode](const instruction& inst) {
        return inst.opcode() == opcode;
    });
}
