#pragma once

#include <ATen/Tensor.h>
#include <c10/core/TensorImpl.h>
#include <c10/util/Exception.h>

namespace at {

// Struct implementing a sparse CSR tensor. It uses three 1-D tensors for
// denoting the data: `crow_indices_`, `col_indices_` and `values_`.
// The `crow_indices_` tensor is a integer tensor of shape `(size(0) + 1)`
// that represents the compressed row indices of the CSR tensor. The
// `col_indices_` tensor is an integer tensor of shape `(nnz())`
// that explicitly stores the column indices of each value of the sparse
// tensor. The `values_` tensor can be of any pytorch-supported data type
// and has shape `(nnz())`.
//
// Since the main advantage of the CSR format over the COO format is speed of
// computation, care must be taken to facilitate smooth interfacing of
// these data structures with optimized libraries such as MKL and MAGMA.
// Since the MKL interface for pytorch currently uses indexing with int32
// type, it is important to make sure that the `crow_indices` and `col_indices`
// are of type int32 when calling MKL routines such as SPMM or SPMV.
//
// If not calling MKL, it should be alright to use 64 bit integer tensors
// for indexing.
struct TORCH_API SparseCsrTensorImpl : public TensorImpl {
  Tensor crow_indices_;
  Tensor col_indices_;
  Tensor values_;

 public:
  explicit SparseCsrTensorImpl(at::DispatchKeySet, const caffe2::TypeMeta);

  void resize_(int64_t nnz, IntArrayRef size);
  void resize_as_sparse_csr_tensor_(const Tensor& src);
  void set_member_tensors(
      const Tensor& crow_indices,
      const Tensor& col_indices,
      const Tensor& values,
      IntArrayRef size);

  const Tensor& crow_indices() const { return crow_indices_; }
  const Tensor& col_indices() const { return col_indices_; }
  const Tensor& values() const { return values_; }
  int nnz() { return values_.size(0); }

  c10::intrusive_ptr<TensorImpl> shallow_copy_and_detach(
      const c10::VariableVersion& version_counter,
      bool allow_tensor_metadata_change) const override {
    TORCH_INTERNAL_ASSERT(
        false,
        "shallow_copy_and_detach not implemented for SparseCsrTensorImpl");
  }

  c10::intrusive_ptr<TensorImpl> shallow_copy_and_detach(
      c10::VariableVersion&& version_counter,
      bool allow_tensor_metadata_change) const override {
    TORCH_INTERNAL_ASSERT(
        false,
        "shallow_copy_and_detach not implemented for SparseCsrTensorImpl");
  }

  void shallow_copy_from(const c10::intrusive_ptr<TensorImpl>& impl) override {
    TORCH_INTERNAL_ASSERT(
        false, "shallow_copy_from not implemented for SparseCsrTensorImpl");
  }

 private:
  explicit SparseCsrTensorImpl(
      at::DispatchKeySet key_set,
      const caffe2::TypeMeta data_type,
      at::Tensor crow_indices,
      at::Tensor col_indices,
      at::Tensor values);
};
} // namespace at
