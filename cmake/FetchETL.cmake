if (NOT DEFINED RMDEV_ETL_REPO_URL)
    set(RMDEV_ETL_REPO_URL "https://github.com/ETLCPP/etl")
    message("Using default ETL repository URL: ${RMDEV_ETL_REPO_URL}")
else ()
    message("Using ETL repository URL: ${RMDEV_ETL_REPO_URL}")
endif ()

if (NOT DEFINED RMDEV_ETL_VERSION)
    set(RMDEV_ETL_VERSION "20.41.2")
    message("Using default ETL version: ${RMDEV_ETL_VERSION}")
else ()
    message("Using ETL version: ${RMDEV_ETL_VERSION}")
endif ()

FetchContent_Declare(
    etl
    GIT_REPOSITORY ${RMDEV_ETL_REPO_URL}
    GIT_TAG        ${RMDEV_ETL_VERSION}
)

FetchContent_MakeAvailable(etl)