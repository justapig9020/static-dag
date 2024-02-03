project('A static DAG library', 'cpp', 'c')

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

lib_src = files('src/dag.c')
tests_src = files('test/test.cpp')
example_src = files('example/main.c', 'example/custom_dags.c')

lib_inc = include_directories('include')
example_inc = include_directories('example')

test_exec = executable(
  'dag-test',
  tests_src + lib_src,
  include_directories : lib_inc,
  dependencies : [
    gtest_dep, 
    gmock_dep]
  )

example_exec = executable(
  'dag-example',
  example_src + lib_src,
  include_directories : [lib_inc, example_inc]
  )

test('dag-test', test_exec)
