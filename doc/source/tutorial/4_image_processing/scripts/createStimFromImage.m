function createStimFromImage(inFile, outFile)
if nargin<2,outFile='../input/image.dat';end

pic = PictureStim;
for i=1:numel(inFile)
	pic.add(inFile{i})
end
pic.resize([256 256]);
pic.rgb2gray();
pic.save(outFile);

end