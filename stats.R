#!/usr/bin/Rscript
library(boot)
options(width=200)

# Load all data in the complete.csv file
all_data <- read.csv("complete.csv", header=TRUE)

# Just LTGA and P3
main_two <- subset(all_data, solver == "LTGA" | solver == "P3")

# Get only the largest problem size LTGA was able to solve
largest <- aggregate(length~problem, data=subset(main_two, solver=="LTGA"), FUN=max)
lookup <- largest$length
names(lookup) <- largest$problem

# Get only the largest successful problem size for LTGA
data <- subset(main_two, length == lookup[problem])


# If the optimum was not found, mark this as 1 more than the worst of anyone
data$evaluations[data$fitness<1] <- max(data$evaluations) + 1

compare <- function(problem_name) {
  return(wilcox.test(evaluations~solver,
         data=subset(data, problem==problem_name))$p.value)
}

# Perform the statistical tests for each problem
sapply(levels(data$problem), compare)

median.fun <- function(x, d) {
  # Used in boot strapping
  return(median(x[d]))
}

straper <- function(data_in) {
  # Find the bootstrapped confidence interval
  bootobj <- boot(data=data_in, statistic=median.fun, R = 1000)
  result <- boot.ci(bootobj, type = "norm")$normal
  return(c(lower.ci=result[2], median=median(data_in), upper.ci=result[3]))
}

# Print out the end of run numbers for each problem and optimization method
aggregate(evaluations~solver+problem+length, data=data, FUN=straper)
