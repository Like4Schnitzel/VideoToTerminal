#include "BinaryUtils.hpp"

void BinaryUtils::writeToFile(const std::string fileName, const std::vector<char> bytes, const bool append)
{
    const ulong byteSize = bytes.size()/CHAR_BIT;

    std::ofstream file;
    if (append)
    {
        file.open(fileName, std::ios_base::app | std::ios::binary);
    }
    else
    {
        file.open(fileName, std::ios::out | std::ios::binary);
    }

    if (!file.good())
    {
        throw std::runtime_error("Cannot open file to write to.");
    }
    file.write(&bytes[0], byteSize);
    file.close();
}

void BinaryUtils::writeToFile(const std::string fileName, char* bytes, const ulong byteSize, const bool append, const bool freeArr)
{
    std::ofstream file;
    if (append)
    {
        file.open(fileName, std::ios_base::app | std::ios::binary);
    }
    else
    {
        file.open(fileName, std::ios::out | std::ios::binary);
    }

    if (!file.good())
    {
        throw std::runtime_error("Cannot open file to write to.");
    }
    file.write(bytes, byteSize);
    file.close();

    if (freeArr)
    {
        free(bytes);
    }
}

char* BinaryUtils::numToCharArray(const float num)
{
    union
    {
        float input;
        int output;
    } data;
    data.input = num;
    return numToCharArray(data.output);
}

ulong BinaryUtils::charArrayToUint(const char* arr, const int len)
{
    ulong num = 0;
    int markiplier = 1;

    for (int i = 0; i < len; i++)
    {
        num += (uint8_t) arr[len-i-1] * markiplier;    // going from right to left
        markiplier *= 256;
    }

    return num;
}

float BinaryUtils::charArrayToFloat(const char* arr, const int len)
{
    if (len != 4)
    {
        std::logic_error("Can only convert char arrays of length 4 to float.");
    }

    union {
        unsigned int x;
        float f;
    } temp;
    temp.x = charArrayToUint(arr, len);
    return temp.f;
}

char* BinaryUtils::charInfoToCharArray(const CharInfo ci)
{
    char* result = (char*)malloc(sizeof(CharInfo));
    int index = 0;

    for (cv::Vec3b vec : {ci.foregroundRGB, ci.backgroundRGB})
    {
        for (int i = 0; i < 3; i++)
        {
            result[index] = vec[i];
            index++;
        }
    }

    result[index] = ci.chara;

    return result;
}

// most of this is taken from https://gist.github.com/arq5x/5315739
CharArrayWithSize BinaryUtils::compressBytes(const char* input, const ulong inputLength)
{
    char* out = (char*)malloc(inputLength);
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = inputLength;
    defstream.next_in = (Bytef*) input;
    defstream.avail_out = inputLength;
    defstream.next_out = (Bytef*) out;

    //std::cout << "compressing...\n";
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    // trim output array
    out = (char*)malloc(defstream.total_out);

    CharArrayWithSize output;
    output.size = defstream.total_out;
    output.arr = out;

    return output;
}

char* BinaryUtils::decompressBytes(const char* input, const ulong inputLength, const ulong outputLength)
{
    char* out = (char*)malloc(outputLength);
    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.avail_in = inputLength;
    infstream.next_in = (Bytef*) input;
    infstream.avail_out = outputLength;
    infstream.next_out = (Bytef*) out;

    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    return out;
}
