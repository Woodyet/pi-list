#pragma once

#include "ebu/list/core/memory/bimo.h"
#include "ebu/list/core/io/data_source.h"

//------------------------------------------------------------------------------

namespace ebu_list
{
    class chunked_data_source
    {
    public:
        chunked_data_source(sbuffer_factory_ptr factory, data_source_uptr source);

        // throws if EOF
        oview read_exactly(ptrdiff_t amount);

        // returns a smaller oview if EOF
        oview try_read_exactly(ptrdiff_t amount);

        sbuffer_factory& get_factory() const;

    public:
        sbuffer_factory_ptr factory_;
        data_source_uptr source_;
        oview cache_;
    };
}
