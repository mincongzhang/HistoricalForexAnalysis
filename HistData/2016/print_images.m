
for file_id = 1:312
    %file_id = 2;
    fname = sprintf('%d.csv', file_id);
    M = dlmread(fname);
    %M = dlmread([int2str(2) '.csv']);
    
    [col row] = size(M);
    if col < 70*15
        continue;
    end
    
    time = M(:,1);
    price = M(:,2);
    avg = M(:,3);
    avg_sd = M(:,4)+avg;
    
    hold on;
    plot(time,price, 'DisplayName', 'price', 'Color', [0,0,1]);
    plot(time,avg, 'DisplayName', 'avg', 'Color', [1,0,0]);
    plot(time,avg_sd, ':', 'DisplayName', 'avgsd', 'Color', [1,0,0]);
    legend('show');
    hold off;
     
    %plot and set axis
    time_range = (20000 <= time & time <= 140000);
    axis([20000 140000 min(price(time_range)) max(price(time_range))]);
    
    %set figure position&size
    x0=5;
    y0=50;
    width=3000;
    height=400;
    set(gcf,'units','points','position',[x0,y0,width,height]);

    set(gca,'Color',[0  0  0]);%axis background
    set(gcf,'Color',[1  1  1]);%plot background
    set(gcf,'InvertHardCopy','off'); %keep features
    
    %hight light range
%     hold on
%     hightlight_range = (40000 <= time & time <= 60000);
%     p3 = plot(time(hightlight_range), price(hightlight_range), 'r', 'LineWidth',4);
%     
%     hightlight_range = (90000 <= time & time <= 110000);
%     p2 = plot(time(hightlight_range), price(hightlight_range), 'r', 'LineWidth',4);
%     hold off
%     
     figure_name = sprintf('%d.png', file_id);
     set(gcf, 'PaperPositionMode','auto');   
     saveas(gcf,figure_name);
%     

    close all;
    %pause(2);
end
