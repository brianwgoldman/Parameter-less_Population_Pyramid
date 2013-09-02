#!/usr/bin/Rscript
library(ggplot2)
library(scales)
library(gridExtra)

data <- read.csv("tmp.csv", header=TRUE)

# If the best found in the run is less than 1, mark it as NA
data$evaluations[data$fitness<1] <- NA

cbMine <- c("#CC79A7", "#E69F00", "#56B4E9", "#000000")

SetupImage <- function(image_name, width=1000, height=800) {
  png(image_name, width=width, height=height)
}


my_median <- function(data) {
  worst <- max(data, na.rm=TRUE)
  data[is.na(data)] <- worst + 1
  result <- median(data)
  if(result > worst) {
    result <- NA
  }
  return(result)
}

make_plot <- function(data, problem_name) {
  plt <- ggplot(data = subset(data, problem==problem_name), aes(x = length, y=evaluations, color = solver))
  plt <- plt + geom_line(stat="summary", fun.y=my_median) + geom_point(stat="summary", fun.y=my_median, aes(shape=solver))
  plt <- plt + scale_y_log10(breaks = trans_breaks("log10", function(x) 10^x),
                labels = trans_format("log10", math_format(10^.x)))
  plt <- plt + scale_x_log10(breaks = trans_breaks("log10", function(x) 10^x),
                labels = trans_format("log10", math_format(10^.x)))
  plt <- plt + scale_color_manual(values=cbMine)
  return(plt)
}
p1 <- make_plot(data, "DeceptiveTrap")
p2 <- make_plot(data, "DeceptiveStepTrap")
p3 <- make_plot(data, "HIFF")
p4 <- make_plot(data, "NearestNeighborNK")
p5 <- make_plot(data, "IsingSpinGlass")
p6 <- make_plot(data, "MAXSAT")
p7 <- make_plot(data, "Rastrigin")

tmp <- ggplot_gtable(ggplot_build(p1))
leg <- which(sapply(tmp$grobs, function(x) x$name) == "guide-box")
group_legend <- tmp$grobs[[leg]]
nl <- theme(legend.position="none")

SetupImage("AllPlots.png", 800, 500)
grid.arrange(arrangeGrob(
             arrangeGrob(p1 + nl + labs(title="Deceptive Trap", x=NULL, y=NULL)),
             arrangeGrob(p2 + nl + labs(title="Deceptive Step Trap", x=NULL, y=NULL)),
             arrangeGrob(p3 + nl + labs(title="HIFF", x=NULL, y=NULL)),
             arrangeGrob(p4 + nl + labs(title="Nearest Neighbor NK", x=NULL, y=NULL)),
             arrangeGrob(p5 + nl + labs(title="Ising Spin Glass", x=NULL, y=NULL)),
             arrangeGrob(p6 + nl + labs(title="MAX-SAT", x=NULL, y=NULL)),
             sub="Problem Size\n", left="\nMedian Evaluations To Success", ncol=3),
             group_legend, widths=c(4/5, 1/5), ncol=2)
dev.off()

SetupImage("WithRast.png", 880, 550)
grid.arrange(arrangeGrob(p1 + nl + labs(title="Deceptive Trap", x=NULL, y=NULL)),
             arrangeGrob(p2 + nl + labs(title="Deceptive Step Trap", x=NULL, y=NULL)),
             arrangeGrob(p3 + nl + labs(title="HIFF", x=NULL, y=NULL)),
             arrangeGrob(p7 + nl + labs(title="Rastrigin", x=NULL, y=NULL)),
             arrangeGrob(p4 + nl + labs(title="Nearest Neighbor NK", x=NULL, y=NULL)),
             arrangeGrob(p5 + nl + labs(title="Ising Spin Glass", x=NULL, y=NULL)),
             arrangeGrob(p6 + nl + labs(title="MAX-SAT", x=NULL, y=NULL)),
             group_legend,
             sub="Problem Size\n", left="\nMedian Evaluations To Success", ncol=4)
dev.off()

