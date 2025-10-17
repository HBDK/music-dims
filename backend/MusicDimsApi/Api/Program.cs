using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtistAlbums;
using MusicDimsApi.Endpoints.GetArtists;
using MusicDimsApi.Endpoints.GetPlayer;
using MusicDimsApi.Endpoints.GetRadioChannels;
using MusicDimsApi.Endpoints.GetRoot;
using MusicDimsApi.Endpoints.PostPlayMedia;
using MusicDimsApi.Endpoints.PostPlayPause;
using MusicDimsApi.Endpoints.PostStopPlayBack;
using MusicDimsApi.Endpoints.PostVolumeUp;
using MusicDimsApi.Endpoints.PostVolumeDown;

var builder = WebApplication.CreateBuilder(args);

builder.Configuration.AddJsonFile("appsettings.secrets.json");
builder.AddHomeAssistantClient();
builder.AddGetArtistsEndpointServices();
builder.AddGetArtistAlbumsEndpointServices();
builder.AddGetRadioChannelsEndpointServices();
builder.AddPostStopPlayBackServices();
builder.AddPostVolumeUpServices();
builder.AddPostVolumeDownServices();
builder.AddPostPlayPauseServices();
builder.AddPostPlayMediaServices();
builder.AddGetPlayerEndpoint();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

app.MapGetArtistsEndpoint();
app.MapGetArtistAlbumsEndpoint();
app.MapGetRadioChannelsEndpoint();
app.MapPostStopPlayBackEndpoint();
app.MapPostVolumeUpEndpoint();
app.MapPostVolumeDownEndpoint();
app.MapPostPlayPauseEndpoint();
app.MapPostPlayMediaEndpoint();
app.MapGetRootEndpoint();
app.MapGetPlayerEndpoint();
app.UseSwagger();
app.UseSwaggerUI();
app.Run();