#include <occa/defines.hpp>

#if OCCA_CUDA_ENABLED
#  ifndef OCCA_MODES_CUDA_STREAMTAG_HEADER
#  define OCCA_MODES_CUDA_STREAMTAG_HEADER

#include <cuda.h>

#include <occa/core/streamTag.hpp>

namespace occa {
  namespace cuda {
    class streamTag : public occa::modeStreamTag_t {
    public:
      CUevent cuEvent;

      streamTag(modeDevice_t *modeDevice_,
                CUevent cuEvent_);

      virtual ~streamTag();
    };
  }
}

#  endif
#endif
