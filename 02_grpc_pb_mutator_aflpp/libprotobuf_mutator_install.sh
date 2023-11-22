set -ex

git clone https://github.com/google/libprotobuf-mutator.git
cd libprotobuf-mutator

#sed -i 's/set(CMAKE_CXX_STANDARD 14)/set(CMAKE_CXX_STANDARD 17)/' path/to/your/CMakeLists.txt

git checkout c390388561be36f94a559a4aed7e2fe60470f60b

mkdir build
cd build
export LDFLAGS="-labsl_bad_any_cast_impl -labsl_bad_optional_access -labsl_bad_variant_access -labsl_base -labsl_city -labsl_civil_time -labsl_cord -labsl_cord_internal -labsl_cordz_functions -labsl_cordz_handle -labsl_cordz_info -labsl_cordz_sample_token -labsl_crc_cord_state -labsl_crc_cpu_detect -labsl_crc_internal -labsl_crc32c -labsl_debugging_internal -labsl_demangle_internal -labsl_die_if_null -labsl_examine_stack -labsl_exponential_biased -labsl_failure_signal_handler -labsl_flags -labsl_flags_commandlineflag -labsl_flags_commandlineflag_internal -labsl_flags_config -labsl_flags_internal -labsl_flags_marshalling -labsl_flags_parse -labsl_flags_private_handle_accessor -labsl_flags_program_name -labsl_flags_reflection -labsl_flags_usage -labsl_flags_usage_internal -labsl_graphcycles_internal -labsl_hash -labsl_hashtablez_sampler -labsl_int128 -labsl_leak_check -labsl_log_entry -labsl_log_flags -labsl_log_globals -labsl_log_initialize -labsl_log_internal_check_op -labsl_log_internal_conditions -labsl_log_internal_format -labsl_log_internal_globals -labsl_log_internal_log_sink_set -labsl_log_internal_message -labsl_log_internal_nullguard -labsl_log_internal_proto -labsl_log_severity -labsl_log_sink -labsl_low_level_hash -labsl_malloc_internal -labsl_periodic_sampler -labsl_random_distributions -labsl_random_internal_distribution_test_util -labsl_random_internal_platform -labsl_random_internal_pool_urbg -labsl_random_internal_randen -labsl_random_internal_randen_hwaes -labsl_random_internal_randen_hwaes_impl -labsl_random_internal_randen_slow -labsl_random_internal_seed_material -labsl_random_seed_gen_exception -labsl_random_seed_sequences -labsl_raw_hash_set -labsl_raw_logging_internal -labsl_scoped_set_env -labsl_spinlock_wait -labsl_stacktrace -labsl_status -labsl_statusor -labsl_str_format_internal -labsl_strerror -labsl_strings -labsl_strings_internal -labsl_symbolize -labsl_synchronization -labsl_throw_delegate -labsl_time -labsl_time_zone -lutf8_validity $LDFLAGS"

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

cmake .. -GNinja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_CXX_FLAGS="-fPIC"
ninja check

ninja
sudo ninja install

