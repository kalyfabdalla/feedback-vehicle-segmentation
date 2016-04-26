FILE(REMOVE_RECURSE
  "lib/libbackground_estimator.pdb"
  "lib/libbackground_estimator.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/background_estimator.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
