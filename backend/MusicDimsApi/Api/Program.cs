using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtistAlbums;
using MusicDimsApi.Endpoints.GetArtists;

var builder = WebApplication.CreateBuilder(args);
builder.Configuration.AddJsonFile("appsettings.secrets.json");
builder.AddHomeAssistantClient();
builder.AddGetArtistsEndpointServices();
builder.AddGetArtistAlbumsEndpointServices();

var app = builder.Build();

app.MapGetArtistsEndpoint();
app.MapGetArtistAlbumsEndpoint();
app.Run();