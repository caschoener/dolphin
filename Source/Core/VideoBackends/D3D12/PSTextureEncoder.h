// Copyright 2011 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <map>
#include <vector>

#include "Common/CommonTypes.h"
#include "VideoBackends/D3D12/D3DBase.h"
#include "VideoCommon/VideoCommon.h"

namespace DX12
{
class PSTextureEncoder final
{
public:
  PSTextureEncoder();

  void Init();
  void Shutdown();
  void Encode(u8* dst, u32 format, u32 native_width, u32 bytes_per_row, u32 num_blocks_y,
              u32 memory_stride, bool is_depth_copy, const EFBRectangle& src_rect,
              bool is_intensity, bool scale_by_half);

private:
  bool m_ready = false;

  ID3D12Resource* m_out = nullptr;
  D3D12_CPU_DESCRIPTOR_HANDLE m_out_rtv_cpu = {};

  ID3D12Resource* m_out_readback_buffer = nullptr;

  ID3D12Resource* m_encode_params_buffer = nullptr;
  void* m_encode_params_buffer_data = nullptr;

  D3D12_SHADER_BYTECODE SetStaticShader(unsigned int dst_format, bool is_depth_copy,
                                        bool is_intensity, bool scale_by_half);

  using ComboKey = unsigned int;  // Key for a shader combination
  static ComboKey MakeComboKey(unsigned int dst_format, bool is_depth_copy, bool is_intensity,
                               bool scale_by_half)
  {
    return (dst_format << 4) | (is_depth_copy << 2) | (is_intensity ? (1 << 1) : 0) |
           (scale_by_half ? (1 << 0) : 0);
  }

  using ComboMap = std::map<ComboKey, D3D12_SHADER_BYTECODE>;
  ComboMap m_static_shaders_map;
  std::vector<ID3DBlob*> m_static_shaders_blobs;
};
}
