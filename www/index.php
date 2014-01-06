<?php

if (isset($_POST['register'])) {

    $pdo = new PDO('mysql:host=localhost;dbname=skypy', 'root', '');

    $error = [];

    if (!preg_match('/^([a-z0-9_-]{3,15})$/', $_POST['name'])) {

        $error[] = "Le nom d'utilisateur n'est pas valide. a-Z0-9_-";
    }
    if (filter_var($_POST['email'], FILTER_VALIDATE_EMAIL) == false) {

        $error[] = "L'email n'est pas valide.";
    }
    if ($_POST['password'] != $_POST['password2']) {

        $error[] = "Le mot de passe n'est pas identique.";
    }
    if (count($error) == 0) {

        $insert = $pdo->prepare('INSERT INTO account(id, email, name, pass_hash, online) VALUES(NULL, ?, ?, ?, 0);');
        $insert->execute([
            $_POST['email'],
            $_POST['name'],
            sha1($_POST['password']),
        ]);
        header('Location: download.php');
        exit();
    }
}

?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>Skypy - Discuter c'est trop cool</title>
    <meta name="viewport" content="initial-scale=1.0" />
    <link href='http://fonts.googleapis.com/css?family=Roboto' rel='stylesheet' type='text/css'>
    <link href="all.css" rel="stylesheet">
</head>
<body>
    <header>
        <a href="index.php">
        <img src="logo.png" />
        </a>
    </header>
    <div class="main">
        <h1>S'inscrire, c'est gratuit et pas cher !</h1>
        <?php if (count($error)) { ?>
        <div class="error">
            <?php echo $error[0]; ?>
        </div>
        <?php } ?>
        <form action="" method="post">
        <p><label>Nom d'utilisateur : </label><input type="text" name="name" /></p>
        <p><label>Email : </label><input type="text" name="email" /></p>
        <p><label>Mot de passe : </label><input type="password" name="password" /></p>
        <p><label>Confirmation du mot de passe : </label><input type="password" name="password2" /></p>
        <p class="center">
            <input type="submit" name="register" value="S'inscrire" />
            <a class="btn" href="download.php">Telecharger Skypy</a>
        </p>
        </form>
    </div>
</body>
</html>
