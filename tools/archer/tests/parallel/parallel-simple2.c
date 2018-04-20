/*
 * parallel-simple2.c -- Archer testcase
 */

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is licensed under the LLNS/LLNL "BSD-Like" license.
// See tools/archer/LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//


// RUN: %libarcher-compile-and-run  | FileCheck %s
#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int var = 0;

// Create team of threads so that there is no implicit happens before
// when creating the thread.
#pragma omp parallel num_threads(2)
  {}

  var++;

#pragma omp parallel num_threads(2) shared(var)
  {
    if (omp_get_thread_num() == 1) {
      var++;
    }
  } // implicit barrier

  fprintf(stderr, "DONE\n");
  int error = (var != 2);
  return error;
}

// CHECK-NOT: ThreadSanitizer: data race
// CHECK-NOT: ThreadSanitizer: reported
// CHECK: DONE
