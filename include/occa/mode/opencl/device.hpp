#include <occa/defines.hpp>

#if OCCA_OPENCL_ENABLED
#  ifndef OCCA_MODES_OPENCL_DEVICE_HEADER
#  define OCCA_MODES_OPENCL_DEVICE_HEADER

#include <occa/core/device.hpp>
#include <occa/mode/opencl/headers.hpp>

namespace occa {
  namespace opencl {
    class info_t;

    class device : public occa::modeDevice_t {
      friend cl_context getContext(occa::device device);

    private:
      mutable hash_t hash_;

    public:
      int platformID, deviceID;

      cl_device_id clDevice;
      cl_context clContext;

      device(const occa::properties &properties_);
      virtual ~device();

      virtual void finish() const;

      virtual bool hasSeparateMemorySpace() const;

      virtual hash_t hash() const;

      virtual hash_t kernelHash(const occa::properties &props) const;

      //---[ Stream ]-------------------
      virtual modeStream_t* createStream(const occa::properties &props);

      virtual streamTag tagStream();
      virtual void waitFor(streamTag tag);
      virtual double timeBetween(const streamTag &startTag,
                                 const streamTag &endTag);

      cl_command_queue& getCommandQueue() const;
      //================================

      //---[ Kernel ]-------------------
      bool parseFile(const std::string &filename,
                     const std::string &outputFile,
                     const std::string &hostOutputFile,
                     const occa::properties &kernelProps,
                     lang::kernelMetadataMap &hostMetadata,
                     lang::kernelMetadataMap &deviceMetadata);

      virtual modeKernel_t* buildKernel(const std::string &filename,
                                        const std::string &kernelName,
                                        const hash_t kernelHash,
                                        const occa::properties &kernelProps);

      modeKernel_t* buildOKLKernelFromBinary(info_t &clInfo,
                                             const std::string &hashDir,
                                             const std::string &kernelName,
                                             lang::kernelMetadataMap &hostMetadata,
                                             lang::kernelMetadataMap &deviceMetadata,
                                             const occa::properties &kernelProps,
                                             io::lock_t lock);

      modeKernel_t* buildLauncherKernel(const std::string &hashDir,
                                        const std::string &kernelName,
                                        lang::kernelMetadata &hostMetadata);

      virtual modeKernel_t* buildKernelFromBinary(const std::string &filename,
                                                  const std::string &kernelName,
                                                  const occa::properties &kernelProps);
      //================================

      //---[ Memory ]-------------------
      virtual modeMemory_t* malloc(const udim_t bytes,
                                   const void *src,
                                   const occa::properties &props);

      virtual modeMemory_t* mappedAlloc(const udim_t bytes,
                                        const void *src,
                                        const occa::properties &props);

      virtual udim_t memorySize() const;
      //================================
    };
  }
}

#  endif
#endif
