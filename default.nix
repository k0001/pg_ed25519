{ stdenv, postgresql }:

let
src-ed25519-donna = builtins.fetchGit {
  url = "https://github.com/floodyberry/ed25519-donna";
  rev = "8757bd4cd209cb032853ece0ce413f122eef212c";
};

in
stdenv.mkDerivation {
  name = "pg_ed25519-0.0.1";
  src = ./.;
  buildInputs = [ postgresql ];
  NIX_CFLAGS_COMPILE = "-I${src-ed25519-donna}";
  installPhase = ''
    mkdir -p $out/bin # For buildEnv to setup proper symlinks. See #22653
    mkdir -p $out/{lib,share/postgresql/extension}

    cp *.so      $out/lib
    cp *.sql     $out/share/postgresql/extension
    cp *.control $out/share/postgresql/extension
  '';
  meta = {
    homepage = "https://github.com/k0001/pg_ed25519";
    description = "Ed25519 tools for PostgreSQL";
    license = stdenv.lib.licenses.publicDomain;
    platforms = stdenv.lib.platforms.linux;
  };
}
