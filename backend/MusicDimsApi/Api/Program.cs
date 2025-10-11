using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtists;

var builder = WebApplication.CreateBuilder(args);
builder.Configuration.AddJsonFile("appsettings.secrets.json");
builder.AddHomeAssistantClient();
builder.AddGetArtistsEndpointServices();

var app = builder.Build();

app.MapGetArtistsEndpoint();
app.Run();