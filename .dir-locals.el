;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((nil . ((project-test-value . (purecopy "ctest --preset ninja-vcpkg"))
         (project-run-value . "builds/ninja-vcpkg/src/tiny-render_RUN")))
 (c++-mode . ((mode . clang-format+)
              (compile-command . "cmake --build --preset ninja-vcpkg")))
 (c-mode . ((mode . clang-format+)
            (compile-command . "cmake --build --preset ninja-vcpkg"))))
