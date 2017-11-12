#include <core/phyr_reporter.h>

namespace phyr {

// Definitions for ProgressReporterException
const char* ProgressReporterException::DEF_ERR = "ProgressReporter already in use";

// Definitions for ProgressReporter
ProgressReporter* ProgressReporter::pr_obj = nullptr;

}  // namespace phyr
