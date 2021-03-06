clear 
ueConfig=struct('NCellID',1,'NULRB',6,'NSubframe',1,'RNTI',18,'CyclicPrefixUL','Normal','NTxAnts',1);
puschConfig=struct('NLayers',1,'OrthCover','Off','PRBSet',[1:4]','Modulation','QPSK','RV',0,'Shortened',0);

addpath('../../debug/srslte/lib/phch/test')

 TBs=0:13:211;
 cqilen=[0, 8, 17];
 mods={'QPSK','16QAM','64QAM'};
 rvs=0;
 betas=[2.0 2.5 6.25];
 
for i=1:length(TBs)
    for m=1:length(mods)
        for r=1:length(rvs)
            for bcqi=1:length(betas)
                for bri=1:length(betas)
                    for back=1:length(betas)          
                        for c=1:length(cqilen)
                            
                            trblkin=randi(2,TBs(i),1)-1;
    
                            puschConfig.Modulation = mods{m};
                            puschConfig.RV = rvs(r);
                            puschConfig.BetaCQI = betas(bcqi); 
                            puschConfig.BetaRI = betas(bri);
                            puschConfig.BetaACK = betas(back);

                            if (betas(bri)>0)
                                ri_bit=randi(2,1,1)-1;
                            else
                                ri_bit=[];
                            end
                            if (betas(back)>0)
                                ack_bit=randi(2,1,1)-1;
                            else
                                ack_bit=[];
                            end
                            
                            if (cqilen(c)>0 || TBs(i)>0)
                                [cw, info]=lteULSCH(ueConfig,puschConfig,trblkin,ones(1,cqilen(c)),ri_bit,ack_bit,[]);
                                cw_mat=ltePUSCH(ueConfig,puschConfig,cw);
                                idx=ltePUSCHIndices(ueConfig,puschConfig);
                                subframe_mat = lteULResourceGrid(ueConfig);
                                subframe_mat(idx)=cw_mat;
                                waveform = lteSCFDMAModulate(ueConfig,subframe_mat,0);
                               
                                [waveform_lib, subframe_lib, cwlib]=srslte_pusch_encode(ueConfig,puschConfig,trblkin,ones(1,cqilen(c)),ri_bit,ack_bit);
                                err=mean(abs(waveform-waveform_lib));
                                if (err > 10^-6)
                                  disp(err)    
                                  error('Error!');
                                end
                            end
                        end
                    end
                end
            end
        end
    end
end

if (length(TBs) == 1)
    %disp(info)
    %n=1:length(mat);
    %plot(abs(double(mat)-double(lib)))
    %plot(n,real(lib(n)),n,real(mat(n)))
    plot(abs(waveform_lib-waveform))
end
