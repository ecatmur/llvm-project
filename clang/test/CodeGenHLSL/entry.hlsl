// RUN: %clang_cc1 -std=hlsl2021 -finclude-default-header -x hlsl -triple \
// RUN:   dxil-pc-shadermodel6.3-compute %s -hlsl-entry foo \
// RUN:   -emit-llvm -disable-llvm-passes -o - | FileCheck %s

// Make sure not mangle entry.
// CHECK:define void @foo()
// Make sure add function attribute.
// CHECK:"hlsl.shader"="compute"
[numthreads(1,1,1)]
void foo() {

}
