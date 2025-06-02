if (NOT DEFINED ${PROJECT_NAME}_ETL_REPO_URL)
    set(${PROJECT_NAME}_ETL_REPO_URL "https://github.com/ETLCPP/etl")
    message("Using default ETL repository URL: ${${PROJECT_NAME}_ETL_REPO_URL}")
else ()
    message("Using ETL repository URL: ${${PROJECT_NAME}_ETL_REPO_URL}")
endif ()

if (NOT DEFINED ${PROJECT_NAME}_ETL_VERSION)
    set(${PROJECT_NAME}_ETL_VERSION "20.41.2")
    message("Using default ETL version: ${${PROJECT_NAME}_ETL_VERSION}")
else ()
    message("Using ETL version: ${${PROJECT_NAME}_ETL_VERSION}")
endif ()

FetchContent_Declare(
    etl
    GIT_REPOSITORY ${${PROJECT_NAME}_ETL_REPO_URL}
    GIT_TAG        ${${PROJECT_NAME}_ETL_VERSION}
)

FetchContent_MakeAvailable(etl)