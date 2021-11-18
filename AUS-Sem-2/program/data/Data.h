#pragma once

namespace data
{

    enum class Source {
        Obce = 0,
        Clenenie = 1,
        ZoskupeneB = 2,
        Byty = 3
    };

    constexpr const char* filePath(Source source)
    {
        constexpr const char* files[4] =
        {
            "program/data/sources/1_ObceSR.csv",
            "program/data/sources/2_UzemneClenenieSR.csv",
            "program/data/sources/3_ZoznamZoskupeniBytovychJednotiekSR.csv",
            "program/data/sources/4_BytoveJednotkySR.csv"
        };

        return files[static_cast<int>(source)];
    }

};
