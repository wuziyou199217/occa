#include <occa/defines.hpp>

#if OCCA_CUDA_ENABLED
#  ifndef OCCA_MODES_CUDA_KERNEL_HEADER
#  define OCCA_MODES_CUDA_KERNEL_HEADER

#include <cuda.h>

#include <occa/core/kernel.hpp>

namespace occa {
  namespace cuda {
    class device;

    class kernel : public occa::modeKernel_t {
      friend class device;

    private:
      CUmodule cuModule;
      CUfunction cuFunction;

      occa::modeKernel_t *launcherKernel;
      std::vector<kernel*> cuKernels;
      mutable std::vector<void*> vArgs;

    public:
      kernel(modeDevice_t *modeDevice_,
             const std::string &name_,
             const std::string &sourceFilename_,
             const occa::properties &properties_);

      kernel(modeDevice_t *modeDevice_,
             const std::string &name_,
             const std::string &sourceFilename_,
             CUmodule cuModule_,
             CUfunction cuFunction_,
             const occa::properties &properties_);

      ~kernel();

      CUstream& getCuStream() const;

      int maxDims() const;
      dim maxOuterDims() const;
      dim maxInnerDims() const;

      void run() const;
      void launcherRun() const;
    };
  }
}

#  endif
#endif
