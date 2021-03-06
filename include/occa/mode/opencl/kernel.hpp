#include <occa/defines.hpp>

#if OCCA_OPENCL_ENABLED
#  ifndef OCCA_MODES_OPENCL_KERNEL_HEADER
#  define OCCA_MODES_OPENCL_KERNEL_HEADER

#include <occa/core/kernel.hpp>
#include <occa/mode/opencl/headers.hpp>
#include <occa/mode/opencl/utils.hpp>

namespace occa {
  namespace opencl {
    class device;

    class kernel : public occa::modeKernel_t {
      friend class device;
      friend cl_kernel getCLKernel(occa::kernel kernel);

    private:
      cl_device_id clDevice;
      cl_kernel clKernel;

      occa::modeKernel_t *launcherKernel;
      std::vector<kernel*> clKernels;

    public:
      kernel(modeDevice_t *modeDevice_,
             const std::string &name_,
             const std::string &sourceFilename_,
             const occa::properties &properties_);

      kernel(modeDevice_t *modeDevice_,
             const std::string &name_,
             const std::string &sourceFilename_,
             cl_device_id clDevice_,
             cl_kernel clKernel_,
             const occa::properties &properties_);

      ~kernel();

      cl_command_queue& getCommandQueue() const;

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
