
for file_id = 1:312
    %file_id = 2;
    fname = sprintf('%d.csv', file_id);
    M = dlmread(fname);
    %M = dlmread([int2str(2) '.csv']);
    
    [col row] = size(M);
    if col < 70 
        continue;
    end
    
    time = M(:,1);
    price = M(:,2);
    
    plot(time,price);
    set(gca,'Color',[0  0  0]);
    
    %plot and set axis
    time_range = (20000 <= time & time <= 140000);
    axis([20000 140000 min(price(time_range)) max(price(time_range))]);
    
    %set figure position&size
    x0=5;
    y0=50;
    width=1000;
    height=400;
    set(gcf,'units','points','position',[x0,y0,width,height]);
    
    %hight light range
    hold on
    hightlight_range = (40000 <= time & time <= 60000);
    p3 = plot(time(hightlight_range), price(hightlight_range), 'r', 'LineWidth',4);
    
    hightlight_range = (90000 <= time & time <= 110000);
    p2 = plot(time(hightlight_range), price(hightlight_range), 'r', 'LineWidth',4);
    hold off
    
    figure_name = sprintf('%d.png', file_id);
    set(gcf, 'PaperPositionMode','auto');   
    saveas(gcf,figure_name);
    
    %pause(2);
end
